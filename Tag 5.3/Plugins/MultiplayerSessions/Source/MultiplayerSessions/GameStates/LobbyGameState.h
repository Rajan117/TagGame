// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "LobbyGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSessionSettingsChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameStarting);

UCLASS()
class MULTIPLAYERSESSIONS_API ALobbyGameState : public AGameState
{
	GENERATED_BODY()

public:
	ALobbyGameState();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_BroadcastSessionSettingsChanged();
	FOnSessionSettingsChanged OnSessionSettingsChangedDelegate;
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_NotifyGameStarting();
	FOnGameStarting OnGameStartingDelegate;
};
