// Fill out your copyright notice in the Description page of Project Settings.


#include "KeybindSettingsTab.h"

#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "KeybindSetting.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UserSettings/EnhancedInputUserSettings.h"

void UKeybindSettingsTab::LoadSettings()
{
	KeybindSettingsBox->ClearChildren();
	KeybindSettings.Empty();
	if (const UEnhancedInputLocalPlayerSubsystem* EISubsystem = GetOwningLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		UserSettings = EISubsystem->GetUserSettings();
	}
	if (!UserSettings) return;
	for (const TPair<FGameplayTag, TObjectPtr<UEnhancedPlayerMappableKeyProfile>>& ProfilePair : UserSettings->GetAllSavedKeyProfiles())
	{
		const TObjectPtr<UEnhancedPlayerMappableKeyProfile>& Profile = ProfilePair.Value;
		for (const TPair<FName, FKeyMappingRow>& RowPair : Profile->GetPlayerMappingRows())
		{
			if (UKeybindSetting* KeybindSetting = CreateWidget<UKeybindSetting>(this, KeybindSettingClass))
			{
				KeybindSetting->Init(this);
				KeybindSetting->Setup(RowPair.Key, RowPair.Value, UserSettings);
				KeybindSettingsBox->AddChild(KeybindSetting);
				KeybindSettings.Add(KeybindSetting);
			}
		}
	}
	StatusText->SetColorAndOpacity(FSlateColor(FLinearColor::Green));
	Super::LoadSettings();
}

void UKeybindSettingsTab::SaveSettings()
{
	if (!CheckMappingsAreValid()) return;
	Super::SaveSettings();
	if (UserSettings) UserSettings->SaveSettings();
	StatusText->SetColorAndOpacity(FSlateColor(FLinearColor::Green));
	StatusText->SetText(FText::FromString(TEXT("Settings Saved")));
	LoadSettings();
}

void UKeybindSettingsTab::ResetSettings()
{
	Super::ResetSettings();
	LoadSettings();
}

bool UKeybindSettingsTab::CheckMappingsAreValid()
{
	TSet<FKey> UsedKeys;
	for (UKeybindSetting* KeybindSetting : KeybindSettings)
	{
		// Check if the keybind has a key assigned
		if (!KeybindSetting->IsKeySelected())
		{
			StatusText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
			StatusText->SetText(FText::FromString(FString::Printf(
			TEXT("Cannot save keybinds, %s has no input assigned."),
				*KeybindSetting->GetActionName().ToString())));
			return false;
		}
		// Check if the keybind has a duplicate key assigned
		for (FKey Key : KeybindSetting->GetSelectedKeys())
		{
			if (UsedKeys.Contains(Key))
			{
				StatusText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
				StatusText->SetText(FText::FromString(FString::Printf(
				TEXT("Cannot save keybinds, %s has uses %s which is already assigned."),
					*KeybindSetting->GetActionName().ToString(), *Key.ToString())));
				return false;
			}
			UsedKeys.Add(Key);
		}
	}
	return true;
}
