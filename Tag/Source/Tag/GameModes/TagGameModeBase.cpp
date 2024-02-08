// Copyright Epic Games, Inc. All Rights Reserved.


#include "TagGameModeBase.h"

void ATagGameModeBase::StartPlay()
{
	Super::StartPlay();
	
	GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Yellow, TEXT("Hello World, this is the TagGameMode!"));
}
