// Fill out your copyright notice in the Description page of Project Settings.


#include "TagRoundBasedGameState.h"

void ATagRoundBasedGameState::Multicast_BroadcastRoundStart_Implementation(float RoundTime)
{
	OnRoundStartedDelegate.Broadcast(RoundTime);
}

void ATagRoundBasedGameState::Multicast_BroadcastRoundEnd_Implementation(float IntervalTime)
{
	OnRoundEndedDelegate.Broadcast(IntervalTime);
}

void ATagRoundBasedGameState::Multicast_BroadcastPlayerEliminated_Implementation(ATagPlayerState* EliminatedPlayer)
{
	OnPlayerEliminatedDelegate.Broadcast(EliminatedPlayer);
}
