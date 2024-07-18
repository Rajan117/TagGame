// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TagGameMode.h"
#include "TagRoundBasedGameMode.generated.h"

class ATagRoundBasedGameState;
/**
 * 
 */
UCLASS()
class TAG_API ATagRoundBasedGameMode : public ATagGameMode
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	
protected:
	//Round State
	virtual void HandleTick(float DeltaSeconds) override;
	virtual void StartGame() override;
	virtual void StartRound();
	virtual void EndRound();

	void SwitchPlayerToSpectator(ATagPlayerController* TagPlayerController) const;
	
	UPROPERTY()
	ATagRoundBasedGameState* TagRoundBasedGameState;
};
