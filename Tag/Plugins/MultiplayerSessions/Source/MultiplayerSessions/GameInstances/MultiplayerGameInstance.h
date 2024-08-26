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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AdvancedFriendsInterface)
	bool bCallFriendInterfaceEventsOnPlayerControllers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AdvancedFriendsInterface)
	bool bCallIdentityInterfaceEventsOnPlayerControllers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AdvancedFriendsInterface)
	bool bCallVoiceInterfaceEventsOnPlayerControllers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AdvancedVoiceInterface)
	bool bEnableTalkingStatusDelegate;

	virtual void Shutdown() override;
	virtual void Init() override;

	//Invite Callbacks
	
	FOnSessionInviteReceivedDelegate SessionInviteReceivedDelegate;
	FDelegateHandle SessionInviteReceivedDelegateHandle;

	void OnSessionInviteReceivedMaster(const FUniqueNetId & PersonInvited, const FUniqueNetId & PersonInviting, const FString & AppId, const FOnlineSessionSearchResult& SessionToJoin);

	void OnSessionInviteReceived(int32 LocalPlayerNum, FUniqueNetId PersonInviting, const FString& AppId, const FOnlineSessionSearchResult& SessionToJoin);

	FOnSessionUserInviteAcceptedDelegate SessionInviteAcceptedDelegate;
	FDelegateHandle SessionInviteAcceptedDelegateHandle;

	void OnSessionInviteAcceptedMaster(const bool bWasSuccessful, int32 LocalPlayer, TSharedPtr<const FUniqueNetId> PersonInviting, const FOnlineSessionSearchResult& SessionToJoin);

	void OnSessionInviteAccepted(int32 LocalPlayerNum, FUniqueNetId PersonInvited, const FOnlineSessionSearchResult& SessionToJoin);

	//Voice Callbacks
	
	void OnPlayerTalkingStateChanged(FUniqueNetId PlayerId, bool bIsTalking);

	void OnPlayerTalkingStateChangedMaster(TSharedRef<const FUniqueNetId> PlayerId, bool bIsTalking);

	FOnPlayerTalkingStateChangedDelegate PlayerTalkingStateChangedDelegate;
	FDelegateHandle PlayerTalkingStateChangedDelegateHandle;

	//Login Callbacks
	
	void OnPlayerLoginChanged(int32 PlayerNum);

	void OnPlayerLoginChangedMaster(int32 PlayerNum);
	FOnLoginChangedDelegate PlayerLoginChangedDelegate;
	FDelegateHandle PlayerLoginChangedDelegateHandle;

	void OnPlayerLoginStatusChanged(int32 PlayerNum, ELoginStatus::Type PreviousStatus, ELoginStatus::Type NewStatus, FUniqueNetId NewPlayerUniqueNetID);

	void OnPlayerLoginStatusChangedMaster(int32 PlayerNum, ELoginStatus::Type PreviousStatus, ELoginStatus::Type NewStatus, const FUniqueNetId & NewPlayerUniqueNetID);
	FOnLoginStatusChangedDelegate PlayerLoginStatusChangedDelegate;
	FDelegateHandle PlayerLoginStatusChangedDelegateHandle;

private:
	UPROPERTY()
	UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;
	
};
