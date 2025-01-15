// Fill out your copyright notice in the Description page of Project Settings.


#include "KeybindSetting.h"

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
}

void UKeybindSetting::LoadSetting()
{
	if (ActionNameText)
	{
		ActionNameText->SetText(FText::FromName(ActionName));
	}
	if (KeySlot1Selector && RowPair.HasAnyMappings())
	{
		KeySlot1Selector->SetSelectedKey(RowPair.Mappings.Array()[0].GetCurrentKey());
	}
	if (KeySlot2Selector && RowPair.Mappings.Num() > 1)
	{
		KeySlot2Selector->SetSelectedKey(RowPair.Mappings.Array()[1].GetCurrentKey());
	}
	else
	{
		KeySlot2Selector->SetVisibility(ESlateVisibility::Hidden);
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
	SaveKeyMapping(RowPair.Mappings.Array()[0].GetDefaultKey(), EPlayerMappableKeySlot::First);
	RowPair.Mappings.Array()[0].ResetToDefault();
	RowPair.Mappings.Array()[1].ResetToDefault();
	if (UserSettings) UserSettings->SaveSettings();
	if (KeySlot1Selector && RowPair.HasAnyMappings())
	{
		KeySlot1Selector->SetSelectedKey(RowPair.Mappings.Array()[0].GetDefaultKey());
	}
	if (KeySlot2Selector && RowPair.Mappings.Num() > 1)
	{
		KeySlot2Selector->SetSelectedKey(RowPair.Mappings.Array()[1].GetDefaultKey());
	}
}

void UKeybindSetting::Setup(const FName InActionName, const FKeyMappingRow& InRowPair, UEnhancedInputUserSettings* InUserSettings)
{
	ActionName = InActionName;
	RowPair = InRowPair;
	UserSettings = InUserSettings;
}

void UKeybindSetting::ResetButtonPressed()
{
	ResetSetting();
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
