// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerListRow.h"

#include "OnlineSubsystem.h"
#include "ServerBrowser.h"
#include "ServerPasswordEntry.h"
#include "Components/Button.h"
#include "Components/Image.h"
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

	if (MapText)
	{
		FString MapName;
		SearchResult.Session.SessionSettings.Get(FName("Map"), MapName);
		MapText->SetText(FText::FromString(MapName));
	}

	if (PlayerCountText)
	{
		const int MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections + SearchResult.Session.SessionSettings.NumPrivateConnections;
		const int CurrentPlayers = SearchResult.Session.NumOpenPublicConnections + SearchResult.Session.NumOpenPrivateConnections;
		const FString Count = FString::FromInt(CurrentPlayers) + "/" + FString::FromInt(MaxPlayers);
		PlayerCountText->SetText(FText::FromString(Count));

		if (CurrentPlayers>=MaxPlayers) JoinButton->SetIsEnabled(false);
	}

	if (PingText)
	{
		const int Ping = SearchResult.PingInMs;
		PingText->SetText(FText::FromString(FString::FromInt(Ping)));
	}
	
	if (PasswordImage)
	{
		PasswordImage->SetVisibility(ESlateVisibility::Hidden);
		SearchResult.Session.SessionSettings.Get(FName("Password"), Password);
		if (!Password.IsEmpty())
		{
			PasswordImage->SetVisibility(ESlateVisibility::Visible);
		}
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

void UServerListRow::ShowLoadingWidget()
{
	if (LoadingWidgetClass)
	{
		if (UUserWidget* LoadingWidgetRef = CreateWidget<UUserWidget>(this, LoadingWidgetClass))
		{
			LoadingWidgetRef->AddToViewport();
		}
	}
}

void UServerListRow::JoinButtonClicked()
{
	if (Password.IsEmpty())
	{
		if (MultiplayerSessionsSubsystem)
		{
			ShowLoadingWidget();
			MultiplayerSessionsSubsystem->JoinSession(SearchResult);
		}
	}
	else if (PasswordEntryWidgetClass)
	{
		if (UServerPasswordEntry* PasswordEntryWidgetRef = CreateWidget<UServerPasswordEntry>(this, PasswordEntryWidgetClass))
		{
			PasswordEntryWidgetRef->SpawnInit(Password);
			PasswordEntryWidgetRef->AddToViewport();
			PasswordEntryWidgetRef->OnPasswordSubmittedDelegate.AddDynamic(this, &UServerListRow::OnServerPasswordSubmitted);
		}
	}
}

void UServerListRow::OnServerPasswordSubmitted(FString SubmittedPassword)
{
	if (MultiplayerSessionsSubsystem && SubmittedPassword == Password)
	{
		ShowLoadingWidget();
		MultiplayerSessionsSubsystem->JoinSession(SearchResult);
	}
}

void UServerListRow::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
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

			if (APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController())
			{
				if (OwningBrowser) OwningBrowser->RemoveFromParent();
				PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
			}
		}
	}
}
