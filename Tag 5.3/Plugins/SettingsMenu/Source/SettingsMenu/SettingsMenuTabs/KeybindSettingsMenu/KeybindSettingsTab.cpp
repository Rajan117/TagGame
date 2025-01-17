// Fill out your copyright notice in the Description page of Project Settings.


#include "KeybindSettingsTab.h"

#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "KeybindSetting.h"
#include "Components/VerticalBox.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UserSettings/EnhancedInputUserSettings.h"

void UKeybindSettingsTab::LoadSettings()
{
	KeybindSettingsBox->ClearChildren();
	if (const UEnhancedInputLocalPlayerSubsystem* EISubsystem = GetOwningLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		UserSettings = EISubsystem->GetUserSettings();
	}
	if (!UserSettings) return;
	for (const TPair<FGameplayTag, TObjectPtr<UEnhancedPlayerMappableKeyProfile>>& ProfilePair : UserSettings->GetAllSavedKeyProfiles())
	{
		const FGameplayTag& ProfileName = ProfilePair.Key;
		const TObjectPtr<UEnhancedPlayerMappableKeyProfile>& Profile = ProfilePair.Value;
		for (const TPair<FName, FKeyMappingRow>& RowPair : Profile->GetPlayerMappingRows())
		{
			if (UKeybindSetting* KeybindSetting = CreateWidget<UKeybindSetting>(this, KeybindSettingClass))
			{
				KeybindSetting->Init(this);
				KeybindSetting->Setup(RowPair.Key, RowPair.Value, UserSettings);
				KeybindSettingsBox->AddChild(KeybindSetting);
			}
		}
	}
	Super::LoadSettings();
}

void UKeybindSettingsTab::SaveSettings()
{
	Super::SaveSettings();
	if (UserSettings) UserSettings->SaveSettings();
	LoadSettings();
}

void UKeybindSettingsTab::ResetSettings()
{
	Super::ResetSettings();
}
