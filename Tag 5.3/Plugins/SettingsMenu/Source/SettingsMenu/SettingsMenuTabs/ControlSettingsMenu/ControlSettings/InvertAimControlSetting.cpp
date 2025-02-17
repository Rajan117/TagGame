// Fill out your copyright notice in the Description page of Project Settings.


#include "InvertAimControlSetting.h"

#include "EnhancedInputSubsystems.h"
#include "Components/CheckBox.h"
#include "SettingsMenu/UserSettings/ExtendedEnhancedInputUserSettings.h"

void UInvertAimControlSetting::LoadSetting()
{
	Super::LoadSetting();
	if (const UEnhancedInputLocalPlayerSubsystem* EISubsystem = GetOwningLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		UserSettings = EISubsystem->GetUserSettings<UExtendedEnhancedInputUserSettings>();
	}
	if (!UserSettings) return;
	const bool bInvertAim = UserSettings->GetInvertAim();
	InvertAimCheckBox->SetIsChecked(bInvertAim);
}

void UInvertAimControlSetting::SaveSetting()
{
	Super::SaveSetting();
	const bool bInvertAim = InvertAimCheckBox->GetCheckedState() == ECheckBoxState::Checked;
	if (const UEnhancedInputLocalPlayerSubsystem* EISubsystem = GetOwningLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		UserSettings = EISubsystem->GetUserSettings<UExtendedEnhancedInputUserSettings>();
	}
	if (!UserSettings) return;
	UserSettings->SetInvertAim(bInvertAim);
	LoadSetting();
}

void UInvertAimControlSetting::ResetSetting()
{
	Super::ResetSetting();
	if (const UEnhancedInputLocalPlayerSubsystem* EISubsystem = GetOwningLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		UserSettings = EISubsystem->GetUserSettings<UExtendedEnhancedInputUserSettings>();
	}
	if (!UserSettings) return;
	UserSettings->SetInvertAim(false);
	LoadSetting();
}
