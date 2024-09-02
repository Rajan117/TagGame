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

	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);
	}
}

void UMultiplayerGameInstance::OnSessionUserInviteAccepted(const bool bWasSuccessful, int32 LocalPlayerNum, FUniqueNetIdPtr PersonInvited,
	const FOnlineSessionSearchResult& SessionToJoin)
{
	if (!MultiplayerSessionsSubsystem) return;
	MultiplayerSessionsSubsystem->JoinSession(SessionToJoin);
}

void UMultiplayerGameInstance::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
	if(Result != EOnJoinSessionCompleteResult::Success)
	{
		UKismetSystemLibrary::PrintString(this, LexToString(Result));
		return;
	}
	
	if (const IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
	{
		if (const IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface(); SessionInterface.IsValid())
		{
			FString Address;
			SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);

			if (APlayerController* PlayerController = GetFirstLocalPlayerController())
			{
				PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
			}
		}
	}
}
