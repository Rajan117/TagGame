// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TagGameState.h"
#include "TagRoundBasedGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoundStarted, float, RoundTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoundEnded, float, IntervalTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerEliminated, ATagPlayerState*, EliminatedPlayer);
/**
 * 
 */
UCLASS()
class TAG_API ATagRoundBasedGameState : public ATagGameState
{
	GENERATED_BODY()
public:
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_BroadcastRoundStart(float NewRoundTime);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_BroadcastRoundEnd(float IntervalTime);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_BroadcastPlayerEliminated(ATagPlayerState* EliminatedPlayer);
	FOnRoundStarted OnRoundStartedDelegate;
	FOnRoundEnded OnRoundEndedDelegate;
	FOnPlayerEliminated OnPlayerEliminatedDelegate;
};
