// Fill out your copyright notice in the Description page of Project Settings.


#include "KeybindSetting.h"

#include "BlueprintEditor.h"
#include "Components/Button.h"
#include "Components/InputKeySelector.h"
#include "Components/TextBlock.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UserSettings/EnhancedInputUserSettings.h"

void UKeybindSetting::NativeConstruct()
{
	Super::NativeConstruct();
	if (ResetButton)
	{
		ResetButton->OnClicked.AddDynamic(this, &UKeybindSetting::ResetButtonPressed);
	}
	if (SecondaryKeySelector)
	{
		SecondaryKeySelector->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UKeybindSetting::LoadSetting()
{
	if (ActionNameText)	ActionNameText->SetText(FText::FromName(ActionName));
	if (RowPair.HasAnyMappings())
	{
		for (FPlayerKeyMapping Mapping : RowPair.Mappings)
		{
			if (Mapping.GetSlot() == EPlayerMappableKeySlot::First)
			{
				PrimaryMapping = Mapping;
				PrimaryKeySelector->SetSelectedKey(PrimaryMapping.GetCurrentKey());
				PrimaryKeySelector->OnKeySelected.AddDynamic(this, &UKeybindSetting::OnPrimaryKeySelected);
			}
			else if (Mapping.GetSlot() == EPlayerMappableKeySlot::Second)
			{
				SecondaryMapping = Mapping;
				SecondaryKeySelector->SetSelectedKey(SecondaryMapping.GetCurrentKey());
				SecondaryKeySelector->OnKeySelected.AddDynamic(this, &UKeybindSetting::OnSecondaryKeySelected);
				SecondaryKeySelector->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}

void UKeybindSetting::SaveSetting()
{
	if (PrimaryKeySelector && RowPair.HasAnyMappings())
	{
		SaveKeyMapping(PrimaryKeySelector->GetSelectedKey().Key, EPlayerMappableKeySlot::First);
	}
	if (SecondaryKeySelector && RowPair.Mappings.Num() > 1)
	{
		SaveKeyMapping(SecondaryKeySelector->GetSelectedKey().Key, EPlayerMappableKeySlot::Second);
	}
}

void UKeybindSetting::ResetSetting()
{
	FMapPlayerKeyArgs Args = {};
	Args.MappingName = ActionName;
	FGameplayTagContainer FailureReason;
	UserSettings->ResetAllPlayerKeysInRow(Args, FailureReason);
	LoadSetting();
}

void UKeybindSetting::Setup(const FName InActionName, const FKeyMappingRow& InRowPair, UEnhancedInputUserSettings* InUserSettings)
{
	ActionName = InActionName;
	RowPair = InRowPair;
	UserSettings = InUserSettings;
}

TArray<FKey> UKeybindSetting::GetSelectedKeys() const
{
	TArray<FKey> SelectedKeys;
	if (PrimaryKeySelector && PrimaryKeySelector->GetSelectedKey().Key.ToString() != "None")
	{
		SelectedKeys.Add(PrimaryKeySelector->GetSelectedKey().Key);
	}
	if (SecondaryKeySelector && SecondaryKeySelector->GetSelectedKey().Key.ToString() != "None")
	{
		SelectedKeys.Add(SecondaryKeySelector->GetSelectedKey().Key);
	}
	return SelectedKeys;
}

bool UKeybindSetting::IsKeySelected()
{
	return GetSelectedKeys().Num() > 0;
}

void UKeybindSetting::ResetButtonPressed()
{
	ResetSetting();
}

void UKeybindSetting::OnPrimaryKeySelected(FInputChord SelectedKey)
{
	PrimaryMapping.SetCurrentKey(SelectedKey.Key);
	PrimaryKeySelector->SetSelectedKey(PrimaryMapping.GetCurrentKey());
	//SaveKeyMapping(SelectedKey.Key, EPlayerMappableKeySlot::First);
}

void UKeybindSetting::OnSecondaryKeySelected(FInputChord SelectedKey)
{
	SecondaryMapping.SetCurrentKey(SelectedKey.Key);
	SecondaryKeySelector->SetSelectedKey(SecondaryMapping.GetCurrentKey());
	//SaveKeyMapping(SelectedKey.Key, EPlayerMappableKeySlot::Second);
}

void UKeybindSetting::SaveKeyMapping(FKey NewKey, EPlayerMappableKeySlot KeySlot)
{
	FMapPlayerKeyArgs Args = {};
	Args.MappingName = ActionName;
	Args.Slot = KeySlot;
	Args.NewKey = NewKey;
	if (UserSettings)
	{
		FGameplayTagContainer FailureReason;
		UserSettings->MapPlayerKey(Args, FailureReason);
	}
}


