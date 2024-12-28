// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Interfaces/OnlineSessionDelegates.h"
#include "LobbyGameMode.generated.h"

class ALobbyGameState;

UCLASS()
class MULTIPLAYERSESSIONS_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ALobbyGameMode();
	virtual void BeginPlay() override;

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;

private:
	FDelegateHandle OnUpdateSessionCompleteDelegate;
	void OnUpdateSessionComplete(FName SessionName, bool bWasSuccessful);
	UFUNCTION()
	void CallLobbyStateMulticast();
	FTimerHandle BroadcastSessionSettingsUpdatedTimer;

	void UpdatePlayerList();

	UPROPERTY()
	ALobbyGameState* LobbyGameState;
};
