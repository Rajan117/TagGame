// Fill out your copyright notice in the Description page of Project Settings.


#include "TagGameMode.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Tag/Character/TagCharacter.h"
#include "Tag/Controller/TagPlayerController.h"
#include "Tag/GameStates/TagGameState.h"
#include "Tag/PlayerState/TagPlayerState.h"

#include "GameFramework/SpectatorPawn.h"
#include "Kismet/KismetSystemLibrary.h"

namespace MatchState
{
	const FName RoundStart = FName("RoundStart"); //During a round
	const FName RoundEnd = FName("RoundEnd"); //Round interval
	const FName Warmup = FName("Warmup"); //Pre-game warmup period
	const FName InMatch = FName("InMatch"); //Actual game
	const FName PostMatch = FName("PostMatch"); //After the game has ended
}

ATagGameMode::ATagGameMode()
{
	bDelayedStart = true;
	TaggedEffectTag = FGameplayTag::RequestGameplayTag(FName("Effect.Tagged"));
	ChooseTaggerEventTag = FGameplayTag::RequestGameplayTag(FName("Event.ChooseTagger"));
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
	if (MatchState == MatchState::WaitingToStart)
	{
		LoadCountdownTime = LoadTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (LoadCountdownTime <= 0.f && GetNumPlayers()>=2)
		{
			StartMatch();
			SetMatchState(MatchState::Warmup);
		}
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
		TagGameState->RoundStartingTime = RoundStartingTime;
	}
}

void ATagGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	if (ATagPlayerController* TagPlayer = Cast<ATagPlayerController>(NewPlayer))
	{
		RestartPlayer(TagPlayer);
	}
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
		  WarmupTime-GetWorld()->GetTimeSeconds(),
		  false
		);
	}
	else if (MatchState == MatchState::PostMatch)
	{
		GetWorld()->GetTimerManager().SetTimer(
		WarmupTimerHandle,
		this,
		&ATagGameMode::RestartGame,
		RestartGameTime,
		false
		);
	}
}

void ATagGameMode::ChooseTagger()
{
	if (!TagEffectClass) return;
	const int32 RandIndex = FMath::RandHelper( GetNumPlayers());
	int32 CurrentIndex = 0;
	for(FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		if (CurrentIndex == RandIndex)
		{
			if (Iterator->Get())
			{
				if (ATagCharacter* ChosenCharacter = Cast<ATagCharacter>(Iterator->Get()->GetCharacter()))
				{
					if (TryChooseTagger(ChosenCharacter))
					{
						bTaggerChosen = true;
					}
					else
					{
						ChooseTagger();
					}
				}
				else
				{
					ChooseTagger();
				}
				break;
			}
			else
			{
				ChooseTagger();
			}
		}
		CurrentIndex++;
	}
}

bool ATagGameMode::TryChooseTagger(ATagCharacter* ChosenCharacter)
{
	if (!ChosenCharacter || !ChosenCharacter->GetAbilitySystemComponent()) return false;
	
	FGameplayEventData EventData;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		ChosenCharacter,
		ChooseTaggerEventTag,
		EventData
	);
	
	const UAbilitySystemComponent* AbilitySystemComponent = ChosenCharacter->GetAbilitySystemComponent();
	return AbilitySystemComponent->HasMatchingGameplayTag(TaggedEffectTag);
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
