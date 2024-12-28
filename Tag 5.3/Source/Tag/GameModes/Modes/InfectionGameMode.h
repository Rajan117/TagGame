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

protected:
	virtual void HandleTagEvent(ATagCharacter* TaggingCharacter, ATagCharacter* TaggedCharacter, ATagPlayerState* TaggingPlayer, ATagPlayerState* TaggedPlayer) override;

private:
	int32 GetNumUntaggedPlayers();
	
};
