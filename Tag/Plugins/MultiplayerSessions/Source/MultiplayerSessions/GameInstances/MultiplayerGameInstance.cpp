// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "MultiplayerSessions/Subsystems/MultiplayerSessionsSubsystem.h"

UMultiplayerGameInstance::UMultiplayerGameInstance(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
	, SessionInviteReceivedDelegate(FOnSessionInviteReceivedDelegate::CreateUObject(this, &ThisClass::OnSessionInviteReceivedMaster))
	, SessionInviteAcceptedDelegate(FOnSessionUserInviteAcceptedDelegate::CreateUObject(this, &ThisClass::OnSessionInviteAcceptedMaster))
	, PlayerLoginChangedDelegate(FOnLoginChangedDelegate::CreateUObject(this, &ThisClass::OnPlayerLoginChangedMaster))
	, PlayerLoginStatusChangedDelegate(FOnLoginStatusChangedDelegate::CreateUObject(this, &ThisClass::OnPlayerLoginStatusChangedMaster))
{
	MultiplayerSessionsSubsystem = GetSubsystem<UMultiplayerSessionsSubsystem>();
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
	Super::Init();
	
	const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (!OnlineSubsystem) return;
	
	if (const IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface(); SessionInterface.IsValid())
	{
		SessionInviteAcceptedDelegateHandle = SessionInterface->AddOnSessionUserInviteAcceptedDelegate_Handle(SessionInviteAcceptedDelegate);
		SessionInviteReceivedDelegateHandle = SessionInterface->AddOnSessionInviteReceivedDelegate_Handle(SessionInviteReceivedDelegate);
		UKismetSystemLibrary::PrintString(this, "Binded Invite Delegates");
	}

	if (const IOnlineIdentityPtr IdentityInterface = OnlineSubsystem->GetIdentityInterface(); IdentityInterface.IsValid())
	{
		PlayerLoginChangedDelegateHandle = IdentityInterface->AddOnLoginChangedDelegate_Handle(PlayerLoginChangedDelegate);
		//Defaults to player 1
		PlayerLoginStatusChangedDelegateHandle = IdentityInterface->AddOnLoginStatusChangedDelegate_Handle(0, PlayerLoginStatusChangedDelegate);
	}
}

void UMultiplayerGameInstance::OnSessionInviteReceivedMaster(const FUniqueNetId& PersonInvited,
	const FUniqueNetId& PersonInviting, const FString& AppId, const FOnlineSessionSearchResult& SessionToJoin)
{
	UKismetSystemLibrary::PrintString(this, "Invite Received");
}

void UMultiplayerGameInstance::OnSessionInviteAcceptedMaster(const bool bWasSuccessful, int32 LocalPlayer,
	TSharedPtr<const FUniqueNetId> PersonInviting, const FOnlineSessionSearchResult& SessionToJoin)
{
	UKismetSystemLibrary::PrintString(this, "Attempted To Accept Invite");

	if (bWasSuccessful)
	{
		MultiplayerSessionsSubsystem->JoinSession(SessionToJoin);
	}
}

void UMultiplayerGameInstance::OnPlayerLoginChangedMaster(int32 PlayerNum)
{

}

void UMultiplayerGameInstance::OnPlayerLoginStatusChangedMaster(int32 PlayerNum, ELoginStatus::Type PreviousStatus,
	ELoginStatus::Type NewStatus, const FUniqueNetId& NewPlayerUniqueNetID)
{

}
