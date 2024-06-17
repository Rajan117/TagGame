// Fill out your copyright notice in the Description page of Project Settings.


#include "TagGameMode.h"

#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Tag/Character/TagCharacter.h"
#include "Tag/Controller/TagPlayerController.h"
#include "Tag/HUD/HUDElements/GameStartTimer.h"
#include "Tag/PlayerState/TagPlayerState.h"

namespace MatchState
{
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
}

void ATagGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (MatchState == MatchState::WaitingToStart)
	{
		LoadCountdownTime = LoadTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (LoadCountdownTime <= 0.f && Players.Num()>=2)
		{
			StartMatch();
			SetMatchState(MatchState::Warmup);
		}
	}
	else if (MatchState == MatchState::InMatch && MatchTime+WarmupTime-GetWorld()->GetTimeSeconds()+LevelStartingTime < 0)
	{
		SetMatchState(MatchState::PostMatch);
	}
}

void ATagGameMode::StartPlay()
{
	Super::StartPlay();
}

void ATagGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	if(ATagPlayerController* TagPlayer = Cast<ATagPlayerController>(NewPlayer))
	{
		Players.Add(TagPlayer);
		TagPlayer->OnMatchStateSet(MatchState);
		if (MatchState == MatchState::Warmup || MatchState == MatchState::InMatch)
		{
			RestartPlayer(TagPlayer);
		}
	}
}

void ATagGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();
	
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		if (ATagPlayerController* TagPlayerController = Cast<ATagPlayerController>(*Iterator))
		{
			TagPlayerController->OnMatchStateSet(MatchState);
		}
	}
	
	if (MatchState == MatchState::Warmup)
	{
		StartGameStartCountdown();
	}
	else if (MatchState == MatchState::InMatch)
	{
		
	}
	else if (MatchState == MatchState::PostMatch)
	{
		StartGameRestartCountdown();
	}
}

void ATagGameMode::StartGameStartCountdown()
{
	GetWorld()->GetTimerManager().SetTimer(
	  ChooseTaggerHandle,
	  this,
	  &ATagGameMode::StartGame,
	  WarmupTime-GetWorld()->GetTimeSeconds(),
	  false
	);
}

ATagPlayerController* ATagGameMode::ChooseTagger()
{
	if (!TagEffectClass || bTaggerChosen) return nullptr;
	const int32 RandIndex = FMath::RandHelper( Players.Num());
	ATagPlayerController* ChosenPlayer = Players[RandIndex];
	
	if (const ATagCharacter* ChosenCharacter = Cast<ATagCharacter>(ChosenPlayer->GetCharacter()))
	{
		if (UAbilitySystemComponent* AbilitySystemComponent = ChosenCharacter->GetAbilitySystemComponent())
		{
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			if (TagEffectClass)
			{
				const FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(
					TagEffectClass, 0, EffectContext);
				if (NewHandle.IsValid())
				{
					if (const FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->
						ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent); ActiveGEHandle.WasSuccessfullyApplied())
					{
						AbilitySystemComponent->AddGameplayCue(FGameplayTag::RequestGameplayTag(FName("GameplayCue.Tagged")), EffectContext);
						AbilitySystemComponent->ForceReplication();
						bTaggerChosen = true;
						return ChosenPlayer;
					}
					else
					{
						ChooseTagger();
					}
				}
			}
		}
	}
	return nullptr;
}

void ATagGameMode::StartGame()
{
	ChooseTagger();
	RoundStartingTime = GetWorld()->GetTimeSeconds();
	SetMatchState(MatchState::InMatch);
}

void ATagGameMode::StartGameRestartCountdown()
{
	GetWorld()->GetTimerManager().SetTimer(
	ChooseTaggerHandle,
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

void ATagGameMode::AnnounceTag(ATagPlayerState* TaggingPlayer, ATagPlayerState* TaggedPlayer)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (ATagPlayerController* TagPlayerController = Cast<ATagPlayerController>(*It))
		{
			TagPlayerController->BroadcastTag(TaggingPlayer, TaggedPlayer);
		}
	}
}

void ATagGameMode::RemoveTaggedEffect(ATagCharacter* TagCharacter)
{
	if (UAbilitySystemComponent* AbilitySystemComponent = TagCharacter->GetAbilitySystemComponent())
	{
		FGameplayTagContainer TaggedEffectTags = FGameplayTagContainer::EmptyContainer;
		TaggedEffectTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Effect.Tagged")));
		const FGameplayEffectQuery TagEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAllOwningTags(TaggedEffectTags);
		AbilitySystemComponent->RemoveActiveEffects(TagEffectQuery, -1);
		
		//Apply speed boost when player tags another player		
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

bool ATagGameMode::TryTag(ATagCharacter* CharacterToTag)
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
					AbilitySystemComponent->AddGameplayCue(FGameplayTag::RequestGameplayTag(FName("GameplayCue.Tagged")), EffectContext);
					//Temporarily disable tag ability when player is tagged
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
