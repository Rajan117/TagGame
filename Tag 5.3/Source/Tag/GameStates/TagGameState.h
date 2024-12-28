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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoundStarted, float, RoundTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoundEnded, float, IntervalTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerEliminated, ATagPlayerState*, EliminatedPlayer);
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

	//Tag Event Delegate
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_BroadcastTag(ATagPlayerState* TaggingPLayer, ATagPlayerState* TaggedPlayer);
	FOnPlayerTagged OnPlayerTaggedDelegate;
	FOnMatchStateChanged OnMatchStateChangedDelegate;

	//Round Delegates
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_BroadcastRoundStart(float RoundTime);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_BroadcastRoundEnd(float IntervalTime);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_BroadcastPlayerEliminated(ATagPlayerState* EliminatedPlayer);
	FOnRoundStarted OnRoundStartedDelegate;
	FOnRoundEnded OnRoundEndedDelegate;
	FOnPlayerEliminated OnPlayerEliminatedDelegate;
	
	//Timekeeping
	UPROPERTY(Replicated)
	float CurrentRoundTime = 0.f;
	UPROPERTY(Replicated)
	float CurrentIntervalTime = 0.f;
	UPROPERTY(Replicated)
	float WarmupTime = 0.f;
	UPROPERTY(Replicated)
	float RestartTime = 0.f;
	UPROPERTY(Replicated)
	float LevelStartingTime = 0.f;
	UPROPERTY(Replicated)
	float RoundStartingTime = 0.f;
};
