// Fill out your copyright notice in the Description page of Project Settings.


#include "TagGameMode.h"

#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Tag/Character/TagCharacter.h"
#include "Tag/Controller/TagPlayerController.h"
#include "Tag/HUD/HUDElements/GameStartTimer.h"

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
		}
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
	}
	
}

void ATagGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	if (MatchState == MatchState::InProgress)
	{
		RoundStartingTime = GetWorld()->GetTimeSeconds();
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			if (ATagPlayerController* TagPlayerController = Cast<ATagPlayerController>(*Iterator))
			{
				TagPlayerController->OnMatchStateSet(MatchState);
				StartGameStartCountdown();
			}
		}
	}
}

void ATagGameMode::StartGameStartCountdown()
{
	GetWorld()->GetTimerManager().SetTimer(
	  ChooseTaggerHandle,
	  this,
	  &ATagGameMode::ChooseTagger,
	  WarmupTime,
	  false
	);
}

void ATagGameMode::ChooseTagger()
{
	if (!TagEffectClass || bTaggerChosen) return;
	const int32 RandIndex = FMath::RandHelper( Players.Num());
	const ATagPlayerController* ChosenPlayer = Players[RandIndex];
	
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
						bTaggerChosen = true;
						StartGame();
					}
					else
					{
						ChooseTagger();
					}
				}
			}
		}
	}
}

void ATagGameMode::StartGame()
{
	
}
