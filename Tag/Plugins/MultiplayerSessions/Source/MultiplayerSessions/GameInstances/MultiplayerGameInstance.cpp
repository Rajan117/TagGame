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
	const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (!OnlineSubsystem) return;

	if (const IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface(); SessionInterface.IsValid())
	{
		SessionInterface->ClearOnSessionUserInviteAcceptedDelegate_Handle(SessionInviteAcceptedDelegateHandle);
		SessionInterface->ClearOnSessionInviteReceivedDelegate_Handle(SessionInviteReceivedDelegateHandle);
	}
	
	if (bEnableTalkingStatusDelegate)
	{
		if (const IOnlineVoicePtr VoiceInterface = OnlineSubsystem->GetVoiceInterface(); VoiceInterface.IsValid())
		{
			VoiceInterface->ClearOnPlayerTalkingStateChangedDelegate_Handle(PlayerTalkingStateChangedDelegateHandle);
		}
	}

	if (const IOnlineIdentityPtr IdentityInterface = OnlineSubsystem->GetIdentityInterface(); IdentityInterface.IsValid())
	{
		IdentityInterface->ClearOnLoginChangedDelegate_Handle(PlayerLoginChangedDelegateHandle);
		//Defaults to player 1
		IdentityInterface->ClearOnLoginStatusChangedDelegate_Handle(0, PlayerLoginStatusChangedDelegateHandle);
	}
	
	Super::Shutdown();
}

void UMultiplayerGameInstance::Init()
{
	const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (!OnlineSubsystem) return;
	
	if (const IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface(); SessionInterface.IsValid())
	{
		SessionInviteAcceptedDelegateHandle = SessionInterface->AddOnSessionUserInviteAcceptedDelegate_Handle(SessionInviteAcceptedDelegate);

		SessionInviteReceivedDelegateHandle = SessionInterface->AddOnSessionInviteReceivedDelegate_Handle(SessionInviteReceivedDelegate);
	}
	
	if (bEnableTalkingStatusDelegate)
	{
		if (const IOnlineVoicePtr VoiceInterface = OnlineSubsystem->GetVoiceInterface(); VoiceInterface.IsValid())
		{
			PlayerTalkingStateChangedDelegateHandle = VoiceInterface->AddOnPlayerTalkingStateChangedDelegate_Handle(PlayerTalkingStateChangedDelegate);
		}
	}

	if (const IOnlineIdentityPtr IdentityInterface = OnlineSubsystem->GetIdentityInterface(); IdentityInterface.IsValid())
	{
		PlayerLoginChangedDelegateHandle = IdentityInterface->AddOnLoginChangedDelegate_Handle(PlayerLoginChangedDelegate);
		//Defaults to player 1
		PlayerLoginStatusChangedDelegateHandle = IdentityInterface->AddOnLoginStatusChangedDelegate_Handle(0, PlayerLoginStatusChangedDelegate);
	}
	
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
