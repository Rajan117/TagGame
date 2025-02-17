// Fill out your copyright notice in the Description page of Project Settings.


#include "XAimSensitivityControlSetting.h"

#include "EnhancedInputSubsystems.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "SettingsMenu/UserSettings/ExtendedEnhancedInputUserSettings.h"

void UXAimSensitivityControlSetting::NativeConstruct()
{
	Super::NativeConstruct();
	if (SensitivitySlider)
	{
		SensitivitySlider->OnValueChanged.AddDynamic(this, &UXAimSensitivityControlSetting::OnSensitivityChanged);
	}
}

void UXAimSensitivityControlSetting::LoadSetting()
{
	Super::LoadSetting();
	if (const UEnhancedInputLocalPlayerSubsystem* EISubsystem = GetOwningLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		UserSettings = EISubsystem->GetUserSettings<UExtendedEnhancedInputUserSettings>();
	}
	if (!UserSettings) return;
	const float CurrentXAimSensitivity = UserSettings->GetAimSensitivity().X;
	SensitivitySlider->SetValue(CurrentXAimSensitivity);
	OnSensitivityChanged(CurrentXAimSensitivity);
}

void UXAimSensitivityControlSetting::SaveSetting()
{
	Super::SaveSetting();
	const float NewAimSensitivity = SensitivitySlider->GetValue();
	if (const UEnhancedInputLocalPlayerSubsystem* EISubsystem = GetOwningLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		UserSettings = EISubsystem->GetUserSettings<UExtendedEnhancedInputUserSettings>();
	}
	if (!UserSettings) return;
	const float Y = UserSettings->GetAimSensitivity().Y;
	UserSettings->SetAimSensitivity(FVector2d(NewAimSensitivity, Y));
	LoadSetting();
}

void UXAimSensitivityControlSetting::ResetSetting()
{
	Super::ResetSetting();
	if (const UEnhancedInputLocalPlayerSubsystem* EISubsystem = GetOwningLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		UserSettings = EISubsystem->GetUserSettings<UExtendedEnhancedInputUserSettings>();
	}
	if (!UserSettings) return;
	UserSettings->SetAimSensitivity(UserSettings->GetDefaultAimSensitivity());
	LoadSetting();
}

void UXAimSensitivityControlSetting::OnSensitivityChanged(float Value)
{
	SensitivityText->SetText(FText::FromString(FString::SanitizeFloat(Value)));
}
