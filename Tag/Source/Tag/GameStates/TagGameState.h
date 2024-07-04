// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "TagGameState.generated.h"

class ATagPlayerState;
class UMatchEndScreen;
class ATagPlayerController;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMatchStateChanged, FName, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerTagged, ATagPlayerState*, TaggingPlayer, ATagPlayerState*, TaggedPlayer);
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
	
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_BroadcastTag(ATagPlayerState* TaggingPLayer, ATagPlayerState* TaggedPlayer);
	FOnPlayerTagged OnPlayerTaggedDelegate;
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
