// Fill out your copyright notice in the Description page of Project Settings.


#include "TagGameMode.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Tag/Character/TagCharacter.h"
#include "Tag/Controller/TagPlayerController.h"
#include "Tag/GameStates/TagGameState.h"
#include "Tag/PlayerState/TagPlayerState.h"

#include "GameFramework/SpectatorPawn.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Tag/TagGameplayTags.h"

DEFINE_LOG_CATEGORY(GAMEMODE);

namespace MatchState
{
	const FName RoundStart = FName("RoundStart"); // During a round
	const FName RoundEnd = FName("RoundEnd"); // Round interval
	const FName Warmup = FName("Warmup"); // Pre-game warmup period
	const FName InMatch = FName("InMatch"); // Actual game
	const FName PostMatch = FName("PostMatch"); // After the game has ended
}

ATagGameMode::ATagGameMode()
{
	bDelayedStart = true;
}

void ATagGameMode::BeginPlay()
{
	Super::BeginPlay();

	LevelStartingTime = GetWorld()->GetTimeSeconds();

	TagGameState = GetGameState<ATagGameState>();
	if (TagGameState)
	{
		InitGameState();
	}
}

void ATagGameMode::Tick(float DeltaSeconds)
{ 
	Super::Tick(DeltaSeconds);
	HandleTick(DeltaSeconds);
}

void ATagGameMode::HandleTick(float DeltaSeconds)
{
	if (GetNumPlayers() >= 1 && MatchState == MatchState::WaitingToStart)
	{
		StartMatch();
		SetMatchState(MatchState::Warmup);
	}
	else if (MatchState == MatchState::RoundStart &&
		GetWorld()->GetTimeSeconds() - RoundStartingTime >= RoundTime)
	{
		EndRound();
	}
	else if (MatchState == MatchState::RoundEnd &&
	GetWorld()->GetTimeSeconds() - RoundStartingTime >= RoundTime+RoundIntervalTime)
	{
		StartRound();
	}
}

void ATagGameMode::InitGameState()
{
	Super::InitGameState();

	if (TagGameState)
	{
		TagGameState->CurrentRoundTime = RoundTime;
		TagGameState->CurrentIntervalTime = RoundIntervalTime;
		TagGameState->WarmupTime = WarmupTime;
		TagGameState->RestartTime = RestartGameTime;
		TagGameState->LevelStartingTime = LevelStartingTime;
		
	}
}

void ATagGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	if (ATagPlayerController* TagPlayer = Cast<ATagPlayerController>(NewPlayer))
	{
		RestartPlayer(TagPlayer);
	}

	if (GetNumPlayers() >= 1 && MatchState == MatchState::WaitingToStart)
	{
		StartMatch();
		SetMatchState(MatchState::Warmup);
	}

	if (TagGameState) TagGameState->ForceNetUpdate();
}

void ATagGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();
	
	if (MatchState == MatchState::Warmup)
	{
		GetWorld()->GetTimerManager().SetTimer(
		  WarmupTimerHandle,
		  this,
		  &ATagGameMode::StartGame,
		  WarmupTime,
		  false
		);
	}

	if (TagGameState) TagGameState->PhaseStartTime = TagGameState->GetServerWorldTimeSeconds();
}

void ATagGameMode::ChooseTagger()
{
	TArray<ATagCharacter*> Candidates;
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (!PC) continue;
		APawn* Pawn = PC->GetPawn();
		if (!Pawn) continue;
		const ATagCharacter* TagCharacter = Cast<ATagCharacter>(Pawn);
		if (!TagCharacter) continue;
		UAbilitySystemComponent* ASC = TagCharacter->GetAbilitySystemComponent();
		if (!ASC) continue;
		if (ASC->HasMatchingGameplayTag(TagGameplayTags::State_Tagged)) continue;

		Candidates.Add(const_cast<ATagCharacter*>(TagCharacter));
	}
	if (Candidates.Num() == 0) return;

	const int32 RandIndex = FMath::RandHelper(Candidates.Num());
	TryChooseTagger(Candidates[RandIndex]);
}

void ATagGameMode::TryChooseTagger(ATagCharacter* ChosenCharacter)
{
	BoundAbilitySystemComponent = ChosenCharacter->GetAbilitySystemComponent();
	TagEffectAddedHandle = BoundAbilitySystemComponent->RegisterGameplayTagEvent(
		TagGameplayTags::State_Tagged,
		EGameplayTagEventType::AnyCountChange
	).AddUObject(this, &ATagGameMode::OnTagEffectApplied);
	
	FGameplayEventData EventData;
	EventData.Instigator = nullptr;
	EventData.Target = ChosenCharacter;
	EventData.EventTag = TagGameplayTags::Event_Tag;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		ChosenCharacter,
		TagGameplayTags::Event_Tag,
		EventData
	);
}

void ATagGameMode::OnTagEffectApplied(const FGameplayTag Tag, int32 TagCount)
{
	BoundAbilitySystemComponent->UnregisterGameplayTagEvent(
		TagEffectAddedHandle,
		TagGameplayTags::State_Tagged,
		EGameplayTagEventType::AnyCountChange
	);
	if (Tag != TagGameplayTags::State_Tagged) return;
	
	if (TagCount > 0)
	{
		bTaggerChosen = true;
	}	
	else if (TagCount <= 0)
	{
		ChooseTagger();
	}
}


void ATagGameMode::StartGame()
{
	StartRound();
}

void ATagGameMode::StartRound()
{
	ChooseTagger();
	RoundStartingTime = GetWorld()->GetTimeSeconds();
	CurrentRound++;
	SetMatchState(MatchState::RoundStart);
	if (TagGameState) TagGameState->Multicast_BroadcastRoundStart(RoundTime);
}

void ATagGameMode::EndRound()
{
	if (NumRounds > 0 && CurrentRound >= NumRounds)
	{
		SetMatchState(MatchState::PostMatch);
	}
	else
	{
		SetMatchState(MatchState::RoundEnd);
		if (TagGameState) TagGameState->Multicast_BroadcastRoundEnd(RoundIntervalTime);
	}
}

//Tag Events

void ATagGameMode::PlayerTagged(ATagCharacter* TaggingCharacter, ATagCharacter* TaggedCharacter)
{
	if (!TaggingCharacter || !TaggedCharacter) return;
	ATagPlayerState* TaggingPlayer = Cast<ATagPlayerState>(TaggingCharacter->GetPlayerState());
	ATagPlayerState* TaggedPlayer = Cast<ATagPlayerState>(TaggedCharacter->GetPlayerState());
	if (!TaggingPlayer || !TaggedPlayer) return;

	//HandleTagEvent(TaggingCharacter, TaggedCharacter, TaggingPlayer, TaggedPlayer);
	AnnounceTag(TaggingPlayer, TaggedPlayer);
}

void ATagGameMode::AnnounceTag(ATagPlayerState* TaggingPlayer, ATagPlayerState* TaggedPlayer)
{
	if (ATagPlayerController* TaggedPlayerController = Cast<ATagPlayerController>(TaggedPlayer->GetPlayerController()); TaggedPlayerController &&
		!TaggedPlayers.Contains(TaggedPlayerController)) TaggedPlayers.Add(TaggedPlayerController);
	if (TagGameState)
	{
		TagGameState->Multicast_BroadcastTag(TaggingPlayer, TaggedPlayer);
	}
}

void ATagGameMode::SwitchPlayerToSpectator(ATagPlayerController* TagPlayerController) const
{
	if (!TagPlayerController) return;

	if (ATagCharacter* TagCharacter = Cast<ATagCharacter>(TagPlayerController->GetCharacter()))
	{
		TagPlayerController->UnPossess();
		TagCharacter->Destroy();
	}
	
	if (ASpectatorPawn* SpectatorPawn = GetWorld()->SpawnActor<ASpectatorPawn>(ASpectatorPawn::StaticClass(), TagPlayerController->GetSpawnLocation(), FRotator::ZeroRotator))
	{
		TagPlayerController->Possess(SpectatorPawn);
		if (APlayerState* PlayerState = TagPlayerController->PlayerState)
		{
			PlayerState->SetIsSpectator(true);
		}
		TagPlayerController->SetViewTarget(SpectatorPawn);
		TagPlayerController->ChangeState(NAME_Spectating);
	}
}

void ATagGameMode::EliminateTaggedPlayers()
{
	for(FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		if (ATagPlayerController* TagPlayerController = Cast<ATagPlayerController>(Iterator->Get()))
		{
			if (const ATagCharacter* TagCharacter = Cast<ATagCharacter>(TagPlayerController->GetCharacter()))
			{
				if (TagCharacter->GetIsTagged())
				{
					EliminatePlayer(TagPlayerController);
				}
			}
		}
	}
}

void ATagGameMode::EliminatePlayer(ATagPlayerController* TagPlayerController)
{
	AnnounceElimination(TagPlayerController->GetPlayerState<ATagPlayerState>());
	EliminatedPlayers.Add(TagPlayerController);
	if (GetNumPlayers()-EliminatedPlayers.Num() <= 1) SetMatchState(MatchState::PostMatch);
	SwitchPlayerToSpectator(TagPlayerController);
}

void ATagGameMode::AnnounceElimination(ATagPlayerState* EliminatedPLayer) const
{
	if (TagGameState) TagGameState->Multicast_BroadcastPlayerEliminated(EliminatedPLayer);
}
