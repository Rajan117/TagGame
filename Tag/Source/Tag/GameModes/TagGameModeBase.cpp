// Copyright Epic Games, Inc. All Rights Reserved.


#include "TagGameModeBase.h"

#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Tag/Character/TagCharacter.h"
#include "Tag/Controller/TagPlayerController.h"

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
		bTaggerChosen = true;
		GetWorld()->GetTimerManager().SetTimer(
		  ChooseTaggerHandle,
		  this,
		  &ATagGameModeBase::ChooseTagger,
		  3,
		  false
		);
	}
}

void ATagGameModeBase::ChooseTagger()
{
	if (!TagEffectClass) return;
	const int32 RandIndex = FMath::RandHelper( Players.Num());
	ATagPlayerController* ChosenPlayer = Players[RandIndex];
	
	if (ATagCharacter* ChosenCharacter = Cast<ATagCharacter>(ChosenPlayer->GetCharacter()))
	{
		if (UAbilitySystemComponent* AbilitySystemComponent = ChosenCharacter->GetAbilitySystemComponent())
		{
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			if (TagEffectClass)
			{
				FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(TagEffectClass, 0, EffectContext);
				if (NewHandle.IsValid())
				{
					FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
				}
			}
		}
		bTaggerChosen = true;
	}
}
