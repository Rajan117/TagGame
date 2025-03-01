// Fill out your copyright notice in the Description page of Project Settings.


#include "TagGameState.h"

#include "Net/UnrealNetwork.h"

void ATagGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATagGameState, CurrentRoundTime);
	DOREPLIFETIME(ATagGameState, CurrentIntervalTime);
	DOREPLIFETIME(ATagGameState, WarmupTime);
	DOREPLIFETIME(ATagGameState, RestartTime);
	DOREPLIFETIME(ATagGameState, LevelStartingTime);
	DOREPLIFETIME(ATagGameState, RoundStartingTime);
}

void ATagGameState::OnRep_MatchState()
{
	Super::OnRep_MatchState();

	OnMatchStateChangedDelegate.Broadcast(MatchState);
}

void ATagGameState::Multicast_BroadcastTag_Implementation(ATagPlayerState* TaggingPLayer, ATagPlayerState* TaggedPlayer)
{
	OnPlayerTaggedDelegate.Broadcast(TaggingPLayer, TaggedPlayer);
}

void ATagGameState::Multicast_BroadcastRoundStart_Implementation(float RoundTime)
{
	OnRoundStartedDelegate.Broadcast(RoundTime);
}

void ATagGameState::Multicast_BroadcastRoundEnd_Implementation(float IntervalTime)
{
	OnRoundEndedDelegate.Broadcast(IntervalTime);
}

void ATagGameState::Multicast_BroadcastPlayerEliminated_Implementation(ATagPlayerState* EliminatedPlayer)
{
	OnPlayerEliminatedDelegate.Broadcast(EliminatedPlayer);
}
