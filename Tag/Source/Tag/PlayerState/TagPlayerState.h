// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TagPlayerState.generated.h"

class ATagCharacter;
class ATagPlayerController;

DECLARE_DELEGATE_OneParam(FScoreUpdated, float);

/**
 * 
 */
UCLASS()
class TAG_API ATagPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ATagPlayerState();
	
	virtual void OnRep_Score() override;
	void ServerSetScore(float ScoreAmount);

	FScoreUpdated ScoreUpdatedDelegate;
	
private:
	UPROPERTY()
	ATagCharacter* TagCharacter;
	UPROPERTY()
	ATagPlayerController* TagPlayerController;
};
