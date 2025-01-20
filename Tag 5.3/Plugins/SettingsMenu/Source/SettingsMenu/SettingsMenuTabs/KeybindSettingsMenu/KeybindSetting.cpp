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
	if (KeySlot1Selector)
	{
		KeySlot1Selector->OnKeySelected.AddDynamic(this, &UKeybindSetting::OnKeySlot1Selected);
	}
	if (KeySlot2Selector)
	{
		KeySlot2Selector->OnKeySelected.AddDynamic(this, &UKeybindSetting::OnKeySlot2Selected);
	}
}

void UKeybindSetting::LoadSetting()
{
	if (ActionNameText)
	{
		ActionNameText->SetText(FText::FromName(ActionName));
	}
	if (RowPair.HasAnyMappings())
	{
		for (FPlayerKeyMapping Mapping : RowPair.Mappings.Array())
		{
			if (Mapping.GetSlot() == EPlayerMappableKeySlot::First)
			{
				Slot1Mapping = Mapping;
				KeySlot1Selector->SetSelectedKey(Slot1Mapping.GetCurrentKey());
			}
			else if (Mapping.GetSlot() == EPlayerMappableKeySlot::Second)
			{
				bHas2Slots = true;
				Slot2Mapping = Mapping;
				KeySlot2Selector->SetSelectedKey(Slot2Mapping.GetCurrentKey());
			}
		}
		if (!bHas2Slots)
		{
			KeySlot2Selector->SetIsEnabled(false);
			KeySlot2Selector->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UKeybindSetting::SaveSetting()
{
	if (KeySlot1Selector && RowPair.HasAnyMappings())
	{
		SaveKeyMapping(KeySlot1Selector->GetSelectedKey().Key, EPlayerMappableKeySlot::First);
	}
	if (KeySlot2Selector && RowPair.Mappings.Num() > 1)
	{
		SaveKeyMapping(KeySlot2Selector->GetSelectedKey().Key, EPlayerMappableKeySlot::Second);
	}
}

void UKeybindSetting::ResetSetting()
{
	if (Slot1Mapping.IsValid())
	{
		SaveKeyMapping(Slot1Mapping.GetDefaultKey(), EPlayerMappableKeySlot::First);
		KeySlot1Selector->SetSelectedKey(Slot1Mapping.GetDefaultKey());
	}
	if (Slot2Mapping.IsValid())
	{
		SaveKeyMapping(Slot2Mapping.GetDefaultKey(), EPlayerMappableKeySlot::Second);
		KeySlot2Selector->SetSelectedKey(Slot2Mapping.GetDefaultKey());
	}
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
	if (KeySlot1Selector && KeySlot1Selector->GetSelectedKey().Key.ToString() != "None")
	{
		SelectedKeys.Add(KeySlot1Selector->GetSelectedKey().Key);
	}
	if (KeySlot2Selector && KeySlot2Selector->GetSelectedKey().Key.ToString() != "None")
	{
		SelectedKeys.Add(KeySlot2Selector->GetSelectedKey().Key);
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

void UKeybindSetting::OnKeySlot1Selected(FInputChord SelectedKey)
{
	SaveKeyMapping(SelectedKey.Key, EPlayerMappableKeySlot::First);
}

void UKeybindSetting::OnKeySlot2Selected(FInputChord SelectedKey)
{
	SaveKeyMapping(SelectedKey.Key, EPlayerMappableKeySlot::Second);
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


