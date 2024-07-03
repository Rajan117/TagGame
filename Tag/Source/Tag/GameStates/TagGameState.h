// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "TagGameState.generated.h"

class UMatchEndScreen;
class ATagPlayerController;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMatchStateChanged, FName, NewState);
/**
 * 
 */
UCLASS()
class TAG_API ATagGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnRep_MatchState() override;

	FOnMatchStateChanged OnMatchStateChangedDelegate;
	
	//Timekeeping
	UPROPERTY(Replicated)
	float MatchTime = 0.f;
	UPROPERTY(Replicated)
	float WarmupTime = 0.f;
	UPROPERTY(Replicated)
	float RestartTime = 0.f;
	UPROPERTY(Replicated)
	float LevelStartingTime = 0.f;
	UPROPERTY(Replicated)
	float RoundStartingTime = 0.f;
};
