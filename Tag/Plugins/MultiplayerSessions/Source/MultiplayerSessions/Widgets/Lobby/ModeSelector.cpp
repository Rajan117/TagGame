// Fill out your copyright notice in the Description page of Project Settings.


#include "ModeSelector.h"

#include "OnlineSessionSettings.h"
#include "Components/ComboBoxString.h"
#include "Kismet/KismetSystemLibrary.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MultiplayerSessions/GameModes/LobbyGameMode.h"
#include "MultiplayerSessions/GameStates/LobbyGameState.h"

void UModeSelector::NativeConstruct()
{
	Super::NativeConstruct();

	for (const TPair<FString, FString>& Pair : ModeNamesAndURLs)
	{
		ModeComboBox->AddOption(Pair.Key);
	}
	ModeComboBox->SetSelectedIndex(0);

	if (const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		if (SessionInterface = OnlineSubsystem->GetSessionInterface(); SessionInterface.IsValid())
		{
			CurrentSession = SessionInterface->GetNamedSession(NAME_GameSession);
			if (CurrentSession && GetOwningPlayer()->HasAuthority())
			{
				CurrentSession->SessionSettings.Set(FName("MatchType"), ModeComboBox->GetSelectedOption(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
				SessionInterface->UpdateSession(NAME_GameSession, CurrentSession->SessionSettings, true);
			}
		}
	}
	
	if (ModeComboBox)
	{
		if (GetWorld())
		{
			ModeComboBox->SetIsEnabled(UKismetSystemLibrary::IsServer(GetWorld()));
		}
	}

	ModeComboBox->OnSelectionChanged.AddDynamic(this, &UModeSelector::OnSelectedModeChanged);

	if (!GetOwningPlayer()->HasAuthority())
	{
		if (ALobbyGameState* LobbyGameState = Cast<ALobbyGameState>(GetWorld()->GetGameState()))
		{
			LobbyGameState->OnSessionSettingsChangedDelegate.AddDynamic(this, &UModeSelector::OnSessionSettingsChanged);
		}
	}
}

FString UModeSelector::GetSelectedModeURL() const
{
	return *ModeNamesAndURLs.Find(ModeComboBox->GetSelectedOption());
}

FString UModeSelector::GetSelectedMode() const
{
	return ModeComboBox->GetSelectedOption();
}

void UModeSelector::OnSelectedModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (!CurrentSession || !SessionInterface) return;
	CurrentSession->SessionSettings.Set(FName("MatchType"), SelectedItem, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionInterface->UpdateSession(NAME_GameSession, CurrentSession->SessionSettings, true);
}

void UModeSelector::OnSessionSettingsChanged()
{
	UKismetSystemLibrary::PrintString(this, "Session Settings Changed");

	if (!CurrentSession) return;
	FString NewMatchType;
	CurrentSession->SessionSettings.Get(FName("MatchType"), NewMatchType);
	if (ModeNamesAndURLs.Contains(NewMatchType)) ModeComboBox->SetSelectedOption(NewMatchType);
}
