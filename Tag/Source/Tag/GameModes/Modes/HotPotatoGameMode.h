// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tag/GameModes/TagGameMode.h"
#include "HotPotatoGameMode.generated.h"

UCLASS()
class TAG_API AHotPotatoGameMode : public ATagGameMode
{
	GENERATED_BODY()

public:
	AHotPotatoGameMode();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	virtual void EliminateTaggedPlayers();
	

private:
	TArray<ATagPlayerController*> SurvivingPlayers;
	TArray<ATagPlayerController*> EliminatedPlayers;
	UPROPERTY()
	ATagPlayerController* CurrentTaggedPlayer;
	
};
