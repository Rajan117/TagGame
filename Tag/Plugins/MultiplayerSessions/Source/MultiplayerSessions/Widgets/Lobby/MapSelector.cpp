// Fill out your copyright notice in the Description page of Project Settings.


#include "MapSelector.h"

#include "ModeSelector.h"
#include "OnlineSessionSettings.h"
#include "Components/ComboBoxString.h"
#include "Kismet/KismetSystemLibrary.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MultiplayerSessions/GameStates/LobbyGameState.h"

class ALobbyGameState;

FString UMapSelector::GetSelectedMapURL() const
{
	return BaseMapsPath + MapComboBox->GetSelectedOption();
}

FString UMapSelector::GetSelectedMap()
{
	return MapComboBox->GetSelectedOption();
}

void UMapSelector::NativeConstruct()
{
	Super::NativeConstruct();
	if (MapComboBox)
	{
		for (FString MapName : MapNames)
		{
			MapComboBox->AddOption(MapName);
		}
		MapComboBox->SetSelectedIndex(0);
		MapComboBox->OnSelectionChanged.AddDynamic(this, &UMapSelector::OnSelectedMapChanged);
	}
	
	if (const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		if (SessionInterface = OnlineSubsystem->GetSessionInterface(); SessionInterface.IsValid())
		{
			CurrentSession = SessionInterface->GetNamedSession(NAME_GameSession);
			if (CurrentSession)
			{
				if (GetOwningPlayer()->HasAuthority())
				{
					CurrentSession->SessionSettings.Set(FName("Map"), MapComboBox->GetSelectedOption(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
					SessionInterface->UpdateSession(NAME_GameSession, CurrentSession->SessionSettings, true);
					MapComboBox->SetIsEnabled(true);
				}
				else
				{
					FString MapName;
					CurrentSession->SessionSettings.Get(FName("Map"), MapName);
					if (MapNames.Contains(MapName)) MapComboBox->SetSelectedOption(MapName);
					if (ALobbyGameState* LobbyGameState = Cast<ALobbyGameState>(GetWorld()->GetGameState()))
					{
						LobbyGameState->OnSessionSettingsChangedDelegate.AddDynamic(this, &UMapSelector::OnSessionSettingsChanged);
					}
					MapComboBox->SetIsEnabled(false);
				}
			}
		}
	}
}

void UMapSelector::OnSelectedMapChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (!CurrentSession || !SessionInterface) return;
	CurrentSession->SessionSettings.Set(FName("Map"), SelectedItem, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionInterface->UpdateSession(NAME_GameSession, CurrentSession->SessionSettings, true);
}

void UMapSelector::OnSessionSettingsChanged()
{
	if (!CurrentSession) return;
	FString MapName;
	CurrentSession->SessionSettings.Get(FName("Map"), MapName);
	if (MapNames.Contains(MapName)) MapComboBox->SetSelectedOption(MapName);
}
