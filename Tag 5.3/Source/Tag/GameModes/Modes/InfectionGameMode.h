// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tag/GameModes/TagGameMode.h"
#include "InfectionGameMode.generated.h"

UCLASS()
class TAG_API AInfectionGameMode : public ATagGameMode
{
	GENERATED_BODY()

public:
	AInfectionGameMode();

private:
	int32 GetNumUntaggedPlayers();
	
};
