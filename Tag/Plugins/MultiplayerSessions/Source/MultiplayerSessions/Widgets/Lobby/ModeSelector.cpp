// Fill out your copyright notice in the Description page of Project Settings.


#include "ModeSelector.h"

#include "OnlineSessionSettings.h"
#include "Components/ComboBoxString.h"
#include "Kismet/KismetSystemLibrary.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

void UModeSelector::NativeConstruct()
{
	Super::NativeConstruct();

	if (const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		if (const IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface(); SessionInterface.IsValid())
		{
			CurrentSession = SessionInterface->GetNamedSession(NAME_GameSession);
			if (!UKismetSystemLibrary::IsServer(GetWorld()))
			{
				SessionInterface->OnSessionSettingsUpdatedDelegates.AddUObject(this, )
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

	for (const TPair<FString, FString>& Pair : ModeNamesAndURLs)
	{
		ModeComboBox->AddOption(Pair.Key);
	}

	ModeComboBox->OnSelectionChanged.AddDynamic(this, &UModeSelector::OnSelectedModeChanged);
	ModeComboBox->SetSelectedIndex(0);
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
	if (!CurrentSession) return;
	CurrentSession->SessionSettings.Set(FName("MatchType"), SelectedItem);
}

void UModeSelector::OnSessionSettingsUpdated(FName SessionName, const FOnlineSessionSettings& UpdatedSettings)
{
	if (!CurrentSession || SessionName != CurrentSession->SessionName) return;
	FString NewMatchType;
	UpdatedSettings.Get(FName("MatchType"), NewMatchType);

	if (ModeNamesAndURLs.Contains(NewMatchType)) ModeComboBox->SetSelectedOption(NewMatchType);
}
