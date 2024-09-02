// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/Engine.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Online.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemTypes.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Interfaces/OnlineUserInterface.h"
#include "Interfaces/OnlineMessageInterface.h"
#include "Interfaces/OnlinePresenceInterface.h"
#include "Engine/GameInstance.h"
#include "Engine/LocalPlayer.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "UObject/UObjectIterator.h"

#include "MultiplayerGameInstance.generated.h"

class UMultiplayerSessionsSubsystem;

UENUM(BlueprintType)
enum class ELoggedInStatus : uint8
{
	//Not logged in or no local profile selected
	NotLoggedIn,
	//Local profile not logged in
	UsingLocalProfile,
	//Player authenticated by chosen platform
	LoggedIn
};

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMultiplayerGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UMultiplayerGameInstance(const FObjectInitializer& ObjectInitializer);
	virtual void Shutdown() override;
	virtual void Init() override;

private:
	UPROPERTY()
	UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	void OnSessionUserInviteAccepted(const bool bWasSuccessful, int32 LocalPlayerNum, FUniqueNetIdPtr PersonInvited, const FOnlineSessionSearchResult& SessionToJoin);
	void OnJoinSession(EOnJoinSessionCompleteResult::Type Result);

};
