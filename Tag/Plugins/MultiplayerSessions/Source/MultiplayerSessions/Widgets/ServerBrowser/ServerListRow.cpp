// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerListRow.h"

#include "OnlineSubsystem.h"
#include "ServerBrowser.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MultiplayerSessions/Subsystems/MultiplayerSessionsSubsystem.h"

class IOnlineSubsystem;

void UServerListRow::SpawnInitialize(FOnlineSessionSearchResult Result, UServerBrowser* Browser)
{
	SearchResult = Result;
	OwningBrowser = Browser;
	

}

void UServerListRow::NativeConstruct()
{
	Super::NativeConstruct();

	if (JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this, &ThisClass::JoinButtonClicked);
	}

	if (NameText)
	{
		FString Name = SearchResult.Session.OwningUserName;
		NameText->SetText(FText::FromString(Name));
	}

	if (MatchText)
	{
		FString MatchType;
		SearchResult.Session.SessionSettings.Get(FName("MatchType"), MatchType);
		MatchText->SetText(FText::FromString(MatchType));
	}

	if (PlayerCountText)
	{
		const int MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
		const int CurrentPlayers = MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
		const FString Count = FString::FromInt(CurrentPlayers) + "/" + FString::FromInt(MaxPlayers);
		PlayerCountText->SetText(FText::FromString(Count));
	}

	if (PingText)
	{
		const int Ping = SearchResult.PingInMs;
		PingText->SetText(FText::FromString(FString::FromInt(Ping)));
	}


	if (const UGameInstance* GameInstance = GetGameInstance())
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}

	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);
	}
}

void UServerListRow::JoinButtonClicked()
{
	if (MultiplayerSessionsSubsystem) MultiplayerSessionsSubsystem->JoinSession(SearchResult);
}

void UServerListRow::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
	if(Result != EOnJoinSessionCompleteResult::Success)
	{
		UKismetSystemLibrary::PrintString(this, "Failed To Join Session");
		return;
	}
	
	if (const IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
	{
		if (const IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface(); SessionInterface.IsValid())
		{
			FString Address;
			SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);

			if (APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController())
			{
				if (OwningBrowser) OwningBrowser->RemoveFromParent();
				PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
			}
		}
	}
}
