// Copyright Epic Games, Inc. All Rights Reserved.


#include "TagGameModeBase.h"

#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Tag/Character/TagCharacter.h"
#include "Tag/Controller/TagPlayerController.h"
#include "Tag/HUD/HUDElements/GameStartTimer.h"

void ATagGameModeBase::StartPlay()
{
	Super::StartPlay();

	
	
}

void ATagGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	if(ATagPlayerController* TagPlayer = Cast<ATagPlayerController>(NewPlayer))
	{
		Players.Add(TagPlayer);
	}

	if (!bTaggerChosen && Players.Num()>=2)
	{
		StartGameStartCountdown();
	}
}

void ATagGameModeBase::StartGameStartCountdown()
{
	GetWorld()->GetTimerManager().SetTimer(
	  ChooseTaggerHandle,
	  this,
	  &ATagGameModeBase::ChooseTagger,
	  5,
	  false
	);

	if (GameStartTimerClass)
	{
		if (UGameStartTimer* GameStartTimer = CreateWidget<UGameStartTimer>(GetWorld(), GameStartTimerClass))
		{
			GameStartTimer->AddToViewport();
		}
	}
}

void ATagGameModeBase::ChooseTagger()
{
	if (!TagEffectClass) return;
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

void ATagGameModeBase::StartGame()
{
	
}

