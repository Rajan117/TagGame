// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerBrowser.h"

#include "OnlineSessionSettings.h"
#include "ServerListRow.h"
#include "Components/Button.h"
#include "Components/CircularThrobber.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MultiplayerSessions/Subsystems/MultiplayerSessionsSubsystem.h"
#include "MultiplayerSessions/Widgets/MultiplayerMainMenu.h"

void UServerBrowser::NativeConstruct()
{
	Super::NativeConstruct();

	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UServerBrowser::BackButtonClicked);
	}
	if (FindButton)
	{
		FindButton->OnClicked.AddDynamic(this, &UServerBrowser::FindButtonClicked);
	}

	if (LoadSymbol)
	{
		LoadSymbol->SetVisibility(ESlateVisibility::Hidden);
	}
	if (SearchStatusText)
	{
		SearchStatusText->SetText(FText::FromString(FString("Search")));
	}

	if (const UGameInstance* GameInstance = GetGameInstance())
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}

	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->MultiplayerOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessions);
	}
}

void UServerBrowser::BackButtonClicked()
{
	if (MenuClass)
	{
		if (UMultiplayerMainMenu* Menu = CreateWidget<UMultiplayerMainMenu>(this, MenuClass))
		{
			if (BackButton) BackButton->SetIsEnabled(false);
			Menu->AddToViewport();
			Menu->MenuSetup();
			RemoveFromParent();
		}
	}
}

void UServerBrowser::FindButtonClicked()
{
	if (BrowserBox) BrowserBox->ClearChildren();
	StartSearch();
	MultiplayerSessionsSubsystem->FindSessions(10000);
	if (FindText) FindText->SetText(FText::FromString(FString("Refresh")));
}

void UServerBrowser::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
{
	EndSearch();
	if (MultiplayerSessionsSubsystem == nullptr || ! BrowserBox) return;

	if (!bWasSuccessful || SessionResults.Num() == 0) 	UKismetSystemLibrary::PrintString(this, "Failed To Find Sessions");
	
	for (const auto Result : SessionResults)
	{
		if (UServerListRow* Row = CreateWidget<UServerListRow>(this, RowClass))
		{
			Row->SpawnInitialize(Result, this);
			BrowserBox->AddChild(Row);
		}
	}
	
}

void UServerBrowser::StartSearch()
{
	if (BackButton)
	{
		BackButton->SetIsEnabled(false);
	}
	if (FindButton)
	{
		FindButton->SetIsEnabled(false);
	}
	if (SearchStatusText)
	{
		SearchStatusText->SetText(FText::FromString(FString("Searching...")));
	}
	if (LoadSymbol)
	{
		LoadSymbol->SetVisibility(ESlateVisibility::Visible);
	}
}

void UServerBrowser::EndSearch()
{
	if (BackButton)
	{
		BackButton->SetIsEnabled(true);
	}
	if (FindButton)
	{
		FindButton->SetIsEnabled(true);
	}
	if (SearchStatusText)
	{
		SearchStatusText->SetText(FText::FromString(FString("Complete")));
	}
	if (LoadSymbol)
	{
		LoadSymbol->SetVisibility(ESlateVisibility::Hidden);
	}
}