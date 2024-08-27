// Fill out your copyright notice in the Description page of Project Settings.


#include "MapSelector.h"

#include "OnlineSessionSettings.h"
#include "Components/ComboBoxString.h"
#include "Kismet/KismetSystemLibrary.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

FString UMapSelector::GetSelectedMapURL() const
{
	return BaseMapsPath + MapComboBox->GetSelectedOption();
}

void UMapSelector::NativeConstruct()
{
	Super::NativeConstruct();

	if (const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		if (const IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface(); SessionInterface.IsValid())
		{
			CurrentSession = SessionInterface->GetNamedSession(NAME_GameSession);
			if (!UKismetSystemLibrary::IsServer(GetWorld()))
			{
				SessionInterface->OnSessionSettingsUpdatedDelegates.AddUObject(this, &UMapSelector::OnSessionSettingsUpdated);
			}
		}
	}
	
	if (MapComboBox)
	{
		if (GetWorld())
		{
			MapComboBox->SetIsEnabled(UKismetSystemLibrary::IsServer(GetWorld()));
		}
	}

	for (FString MapName : MapNames)
	{
		MapComboBox->AddOption(MapName);
	}

	MapComboBox->OnSelectionChanged.AddDynamic(this, &UMapSelector::OnSelectedMapChanged);
	MapComboBox->SetSelectedIndex(0);
}

void UMapSelector::OnSelectedMapChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (!CurrentSession) return;
	CurrentSession->SessionSettings.Set(FName("Map"), SelectedItem);
}

void UMapSelector::OnSessionSettingsUpdated(FName SessionName, const FOnlineSessionSettings& UpdatedSettings)
{
	if (!CurrentSession || SessionName != CurrentSession->SessionName) return;
	FString NewMatchType;
	UpdatedSettings.Get(FName("Map"), NewMatchType);

	if (MapNames.Contains(NewMatchType)) MapComboBox->SetSelectedOption(NewMatchType);
}
