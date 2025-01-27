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
	if (ClearPrimaryBindingButton)
	{
		ClearPrimaryBindingButton->OnClicked.AddDynamic(this, &UKeybindSetting::ClearPrimaryBindingButtonPressed);
	}
	if (ClearSecondaryBindingButton)
	{
		ClearSecondaryBindingButton->OnClicked.AddDynamic(this, &UKeybindSetting::ClearSecondaryBindingButtonPressed);
	}
	SecondaryKeySelector->SetVisibility(ESlateVisibility::Hidden);
	ClearSecondaryBindingButton->SetVisibility(ESlateVisibility::Hidden);
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
			}
			else if (Mapping.GetSlot() == EPlayerMappableKeySlot::Second)
			{
				SecondaryMapping = Mapping;
				SecondaryKeySelector->SetSelectedKey(SecondaryMapping.GetCurrentKey());
				SecondaryKeySelector->SetVisibility(ESlateVisibility::Visible);
				ClearSecondaryBindingButton->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}

void UKeybindSetting::SaveSetting()
{
	SaveKeyMapping(PrimaryKeySelector->GetSelectedKey().Key, EPlayerMappableKeySlot::First);
	if (SecondaryKeySelector->GetVisibility() == ESlateVisibility::Visible)
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
	if (PrimaryKeySelector->GetSelectedKey().Key != FKey())
	{
		SelectedKeys.Add(PrimaryKeySelector->GetSelectedKey().Key);
	}
	if (SecondaryKeySelector->GetSelectedKey().Key != FKey())
	{
		SelectedKeys.Add(SecondaryKeySelector->GetSelectedKey().Key);
	}
	return SelectedKeys;
}

bool UKeybindSetting::IsKeySelected()
{
	return GetSelectedKeys().Num() > 0;
}

void UKeybindSetting::ClearPrimaryBindingButtonPressed()
{
	PrimaryKeySelector->SetSelectedKey(FInputChord());
}

void UKeybindSetting::ClearSecondaryBindingButtonPressed()
{
	SecondaryKeySelector->SetSelectedKey(FInputChord());
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


