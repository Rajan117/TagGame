// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerBrowser.h"

#include "OnlineSessionSettings.h"
#include "ServerListRow.h"
#include "Components/Button.h"
#include "Components/CircularThrobber.h"
#include "Components/ComboBox.h"
#include "Components/ComboBoxString.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MultiplayerSessions/Subsystems/MultiplayerSessionsSubsystem.h"
#include "MultiplayerSessions/Widgets/MultiplayerMainMenu.h"
#include "MultiplayerSessions/Widgets/Lobby/MapSelector.h"
#include "MultiplayerSessions/Widgets/Lobby/ModeSelector.h"

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

	Search();
}

void UServerBrowser::NativeDestruct()
{
	Super::NativeDestruct();
	
	if (SearchTimeoutHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(SearchTimeoutHandle);
	}
	
	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->MultiplayerOnFindSessionsComplete.Remove(OnFindSessionsCompeteDelegateHandle);
		OnFindSessionsCompeteDelegateHandle.Reset();
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
	Search();
}

void UServerBrowser::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
{
	EndSearch();

	if (MultiplayerSessionsSubsystem == nullptr || ! BrowserBox) return;
	if (!bWasSuccessful) 	UKismetSystemLibrary::PrintString(this, "Failed To Find Sessions");
	
	for (const auto Result : SessionResults)
	{
		if (!FilterResult(Result)) break;
			
		if (UServerListRow* Row = CreateWidget<UServerListRow>(this, RowClass))
		{
			Row->SpawnInitialize(Result, this);
			BrowserBox->AddChild(Row);
		}
	}
}

void UServerBrowser::StartSearch()
{
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
	if (SearchTimeoutHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(SearchTimeoutHandle);
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

void UServerBrowser::Search()
{
	if (BrowserBox) BrowserBox->ClearChildren();
	StartSearch();
	
	if (MultiplayerSessionsSubsystem && !OnFindSessionsCompeteDelegateHandle.IsValid())
	{
		OnFindSessionsCompeteDelegateHandle = MultiplayerSessionsSubsystem->MultiplayerOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessions);
	}
	
	GetWorld()->GetTimerManager().SetTimer(
		SearchTimeoutHandle,
		this,
		&UServerBrowser::OnSearchTimeout,
		SearchTimeoutDuration,
		false
	);
	
	MultiplayerSessionsSubsystem->FindSessions(500);
	FindButtonImage->SetBrushFromTexture(RefreshIcon);
}

void UServerBrowser::OnSearchTimeout()
{
	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->MultiplayerOnFindSessionsComplete.Remove(OnFindSessionsCompeteDelegateHandle);
	}
	
	EndSearch();
	
	if (SearchStatusText)
	{
		SearchStatusText->SetText(FText::FromString(FString("Search Timed Out")));
	}
}

bool UServerBrowser::FilterResult(const FOnlineSessionSearchResult& SessionSearchResult)
{
	//Filter by invite only
	bool bInviteOnly;
	SessionSearchResult.Session.SessionSettings.Get(FName("InviteOnly"), bInviteOnly);
	if (bInviteOnly) return false;
	
	//Filter by mode
	FString MatchType;
	SessionSearchResult.Session.SessionSettings.Get(FName("MatchType"), MatchType);
	if (ModeFilter->GetSelectedOption() != "Any" && MatchType != ModeFilter->GetSelectedOption()) return false;
	
	//Filter by map
	FString MapName;
	SessionSearchResult.Session.SessionSettings.Get(FName("Map"), MapName);
	if (MapFilter->GetSelectedOption() != "Any" && MapName != MapFilter->GetSelectedOption()) return false;
	
	return true;
}
