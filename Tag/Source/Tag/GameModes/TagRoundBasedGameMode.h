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
	UPROPERTY(EditDefaultsOnly)
	int32 NumRounds = 2; //Set to 0 for infinite rounds
	UPROPERTY(EditDefaultsOnly)
	float RoundTime = 30.f;
	UPROPERTY(EditDefaultsOnly)
	float RoundIntervalTime = 5.f;
	int32 CurrentRound = 0;

	void SwitchPlayerToSpectator(ATagPlayerController* TagPlayerController) const;
	
private:
	UPROPERTY()
	ATagRoundBasedGameState* TagRoundBasedGameState;
	
};
