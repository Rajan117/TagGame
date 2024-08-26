// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

UMultiplayerGameInstance::UMultiplayerGameInstance(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
	, bCallFriendInterfaceEventsOnPlayerControllers(true)
	, bCallIdentityInterfaceEventsOnPlayerControllers(true)
	, bCallVoiceInterfaceEventsOnPlayerControllers(true)
	, bEnableTalkingStatusDelegate(true)
	, SessionInviteReceivedDelegate(FOnSessionInviteReceivedDelegate::CreateUObject(this, &ThisClass::OnSessionInviteReceivedMaster))
	, SessionInviteAcceptedDelegate(FOnSessionUserInviteAcceptedDelegate::CreateUObject(this, &ThisClass::OnSessionInviteAcceptedMaster))
	, PlayerTalkingStateChangedDelegate(FOnPlayerTalkingStateChangedDelegate::CreateUObject(this, &ThisClass::OnPlayerTalkingStateChangedMaster))
	, PlayerLoginChangedDelegate(FOnLoginChangedDelegate::CreateUObject(this, &ThisClass::OnPlayerLoginChangedMaster))
	, PlayerLoginStatusChangedDelegate(FOnLoginStatusChangedDelegate::CreateUObject(this, &ThisClass::OnPlayerLoginStatusChangedMaster))
{
}

void UMultiplayerGameInstance::Shutdown()
{
	Super::Shutdown();
}

void UMultiplayerGameInstance::Init()
{
	Super::Init();
}

void UMultiplayerGameInstance::OnSessionInviteReceivedMaster(const FUniqueNetId& PersonInvited,
	const FUniqueNetId& PersonInviting, const FString& AppId, const FOnlineSessionSearchResult& SessionToJoin)
{
}

void UMultiplayerGameInstance::OnSessionInviteAcceptedMaster(const bool bWasSuccessful, int32 LocalPlayer,
	TSharedPtr<const FUniqueNetId> PersonInviting, const FOnlineSessionSearchResult& SessionToJoin)
{
}

void UMultiplayerGameInstance::OnPlayerTalkingStateChangedMaster(TSharedRef<const FUniqueNetId> PlayerId,
	bool bIsTalking)
{
}

void UMultiplayerGameInstance::OnPlayerLoginChangedMaster(int32 PlayerNum)
{
}

void UMultiplayerGameInstance::OnPlayerLoginStatusChangedMaster(int32 PlayerNum, ELoginStatus::Type PreviousStatus,
	ELoginStatus::Type NewStatus, const FUniqueNetId& NewPlayerUniqueNetID)
{
}
