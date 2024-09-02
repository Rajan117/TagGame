// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "MultiplayerSessions/Subsystems/MultiplayerSessionsSubsystem.h"

UMultiplayerGameInstance::UMultiplayerGameInstance(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
}

void UMultiplayerGameInstance::Shutdown()
{
	Super::Shutdown();
}

void UMultiplayerGameInstance::Init()
{
	Super::Init();
	const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (!OnlineSubsystem) return;
	MultiplayerSessionsSubsystem = GetSubsystem<UMultiplayerSessionsSubsystem>();
	
	if (const IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface(); SessionInterface.IsValid())
	{
		SessionInterface->OnSessionUserInviteAcceptedDelegates.AddUObject(this, &UMultiplayerGameInstance::OnSessionUserInviteAccepted);
	}
}

void UMultiplayerGameInstance::OnSessionUserInviteAccepted(const bool bWasSuccessful, int32 LocalPlayerNum, FUniqueNetIdPtr PersonInvited,
	const FOnlineSessionSearchResult& SessionToJoin)
{
	UKismetSystemLibrary::PrintString(this, "Accepted Invite");
	if (!MultiplayerSessionsSubsystem) return;
	MultiplayerSessionsSubsystem->JoinSession(SessionToJoin);
}
