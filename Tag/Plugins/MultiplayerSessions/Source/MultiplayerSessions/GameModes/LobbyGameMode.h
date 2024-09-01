// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Interfaces/OnlineSessionDelegates.h"
#include "LobbyGameMode.generated.h"

class FOnlineSessionSettings;

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
	void OnSessionSettingsUpdated(FName SessionName, const FOnlineSessionSettings& UpdatedSettings);
	FDelegateHandle OnUpdateSessionCompleteDelegate;
	FDelegateHandle OnSessionSettingsUpdatedDelegateHandle;
	void OnUpdateSessionComplete(FName SessionName, bool bWasSuccessful);
	
	void UpdatePlayerList();
};
