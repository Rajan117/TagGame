// Fill out your copyright notice in the Description page of Project Settings.


#include "TagGameMode.h"
#include "Tag/Character/TagCharacter.h"
#include "Tag/Controller/TagPlayerController.h"
#include "Tag/GameStates/TagGameState.h"
#include "Tag/PlayerState/TagPlayerState.h"

#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Kismet/KismetArrayLibrary.h"
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
}

void ATagGameMode::BeginPlay()
{
	Super::BeginPlay();

	LevelStartingTime = GetWorld()->GetTimeSeconds();

	TagGameState = TagGameState == nullptr ? Cast<ATagGameState>(GetGameState<ATagGameState>()) : TagGameState;
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
	else if (MatchState == MatchState::InMatch && RoundTime+WarmupTime-GetWorld()->GetTimeSeconds()+LevelStartingTime < 0)
	{
		SetMatchState(MatchState::PostMatch);
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
		if (MatchState == MatchState::Warmup || MatchState == MatchState::InMatch)
		{
			RestartPlayer(TagPlayer);
		}
	}
}

void ATagGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();
	
	if (MatchState == MatchState::Warmup)
	{
		StartGameStartCountdown();
	}
	else if (MatchState == MatchState::PostMatch)
	{
		StartGameRestartCountdown();
	}
}

void ATagGameMode::StartGameStartCountdown()
{
	GetWorld()->GetTimerManager().SetTimer(
	  WarmupTimerHandle,
	  this,
	  &ATagGameMode::StartGame,
	  WarmupTime-GetWorld()->GetTimeSeconds(),
	  false
	);
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
				if (const ATagCharacter* ChosenCharacter = Cast<ATagCharacter>(Iterator->Get()->GetCharacter()))
				{
					if (TryTag(ChosenCharacter))
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
			ChooseTagger();
		}
		CurrentIndex++;
	}
}

void ATagGameMode::StartGame()
{
	ChooseTagger();
	SetMatchState(MatchState::InMatch);
}

void ATagGameMode::StartGameRestartCountdown()
{
	GetWorld()->GetTimerManager().SetTimer(
	WarmupTimerHandle,
	this,
	&ATagGameMode::RestartGame,
	RestartGameTime,
	false
	);
}

//Tag Events

void ATagGameMode::PlayerTagged(ATagCharacter* TaggingCharacter, ATagCharacter* TaggedCharacter)
{
	if (!TaggingCharacter || !TaggedCharacter) return;
	ATagPlayerState* TaggingPlayer = Cast<ATagPlayerState>(TaggingCharacter->GetPlayerState());
	ATagPlayerState* TaggedPlayer = Cast<ATagPlayerState>(TaggedCharacter->GetPlayerState());
	if (!TaggingPlayer || !TaggedPlayer) return;

	HandleTagEvent(TaggingCharacter, TaggedCharacter, TaggingPlayer, TaggedPlayer);
}

void ATagGameMode::HandleTagEvent(ATagCharacter* TaggingCharacter, ATagCharacter* TaggedCharacter,
	ATagPlayerState* TaggingPlayer, ATagPlayerState* TaggedPlayer)
{
	if (TryTag(TaggedCharacter))
	{
		RemoveTaggedEffect(TaggingCharacter);
		AnnounceTag(TaggingPlayer, TaggedPlayer);
	}
}

void ATagGameMode::AnnounceTag(ATagPlayerState* TaggingPlayer, ATagPlayerState* TaggedPlayer) const
{
	if (TagGameState)
	{
		TagGameState->Multicast_BroadcastTag(TaggingPlayer, TaggedPlayer);
	}
}

void ATagGameMode::RemoveTaggedEffect(const ATagCharacter* TagCharacter)
{
	if (!TagCharacter) return;
	if (UAbilitySystemComponent* AbilitySystemComponent = TagCharacter->GetAbilitySystemComponent())
	{
		FGameplayTagContainer TaggedEffectTags = FGameplayTagContainer::EmptyContainer;
		TaggedEffectTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Effect.Tagged")));
		const FGameplayEffectQuery TagEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAllOwningTags(TaggedEffectTags);
		AbilitySystemComponent->RemoveActiveEffects(TagEffectQuery, -1);
		if (ATagPlayerController* TaggingPlayerController = Cast<ATagPlayerController>(TagCharacter->GetPlayerState()->GetPlayerController()); TaggingPlayerController &&
			TaggedPlayers.Contains(TaggingPlayerController)) TaggedPlayers.Remove(TaggingPlayerController);
		
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		if (SpeedBoostEffectClass)
		{
			if (const FGameplayEffectSpecHandle SpeedBoostHandle = AbilitySystemComponent->MakeOutgoingSpec(SpeedBoostEffectClass, 0, EffectContext); SpeedBoostHandle.IsValid())
			{
				AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*SpeedBoostHandle.Data.Get(), AbilitySystemComponent);
			}
		}
	}
}

bool ATagGameMode::TryTag(const ATagCharacter* CharacterToTag)
{
	if (UAbilitySystemComponent* AbilitySystemComponent = CharacterToTag->GetAbilitySystemComponent())
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		if (TagEffectClass)
		{
			if (const FGameplayEffectSpecHandle TaggedHandle = AbilitySystemComponent->MakeOutgoingSpec(TagEffectClass, 0, EffectContext); TaggedHandle.IsValid())
			{
				if (AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*TaggedHandle.Data.Get(), AbilitySystemComponent).WasSuccessfullyApplied())
				{
					if (ATagPlayerController* TaggedPlayerController = Cast<ATagPlayerController>(CharacterToTag->GetPlayerState()->GetPlayerController()); TaggedPlayerController &&
						!TaggedPlayers.Contains(TaggedPlayerController)) TaggedPlayers.Add(TaggedPlayerController);
					AbilitySystemComponent->AddGameplayCue(FGameplayTag::RequestGameplayTag(FName("GameplayCue.Tagged")), EffectContext);
					if (TagDisabledEffectClass)
					{
						if (const FGameplayEffectSpecHandle TaggedDebuffHandle = AbilitySystemComponent->MakeOutgoingSpec(TagDisabledEffectClass, 0, EffectContext); TaggedDebuffHandle.IsValid())
						{
							AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*TaggedDebuffHandle.Data.Get(), AbilitySystemComponent);
						}
					}
					return true;
				}
			}
		}
	}
	return false;
}
