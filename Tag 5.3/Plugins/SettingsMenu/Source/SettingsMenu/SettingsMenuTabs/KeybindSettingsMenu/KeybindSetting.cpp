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
	if (KeySelector && RowPair.HasAnyMappings())
	{
		KeySelector->SetSelectedKey(RowPair.Mappings.Array()[0].GetCurrentKey());
	}
}

void UKeybindSetting::SaveSetting()
{
	FMapPlayerKeyArgs Args = {};
	Args.MappingName = ActionName;
	Args.Slot = EPlayerMappableKeySlot::First;
	Args.NewKey = KeySelector->GetSelectedKey().Key;
	if (UserSettings)
	{
		FGameplayTagContainer FailureReason;
		UserSettings->MapPlayerKey(Args, FailureReason);
	}
}

void UKeybindSetting::ResetSetting()
{
	FMapPlayerKeyArgs Args = {};
	Args.MappingName = ActionName;
	Args.Slot = EPlayerMappableKeySlot::First;
	Args.NewKey = RowPair.Mappings.Array()[0].GetDefaultKey();
	if (UserSettings)
	{
		FGameplayTagContainer FailureReason;
		UserSettings->UnMapPlayerKey(Args, FailureReason);
	}
	if (KeySelector && RowPair.HasAnyMappings())
	{
		KeySelector->SetSelectedKey(RowPair.Mappings.Array()[0].GetDefaultKey());
	}
}

void UKeybindSetting::Setup(FName InActionName, FKeyMappingRow InRowPair, UEnhancedInputUserSettings* InUserSettings)
{
	ActionName = InActionName;
	RowPair = InRowPair;
	UserSettings = InUserSettings;
}

void UKeybindSetting::ResetButtonPressed()
{
	ResetSetting();
}
