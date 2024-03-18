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

	if (Players.Num()>=2)
	{
		ChooseTagger();	
	}
}

void ATagGameModeBase::ChooseTagger()
{
	if (!TagEffectClass) return;
	
	const int32 RandIndex = FMath::RandRange(0, Players.Num()-1);
	const ATagPlayerController* ChosenPlayer = Players[RandIndex];
	
	if (ATagCharacter* ChosenCharacter = Cast<ATagCharacter>(ChosenPlayer->GetCharacter()))
	{
		ChosenCharacter->Tag();
	}
}
