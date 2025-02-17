// Fill out your copyright notice in the Description page of Project Settings.


#include "YAimSensitivityControlSetting.h"

#include "EnhancedInputSubsystems.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "SettingsMenu/UserSettings/ExtendedEnhancedInputUserSettings.h"

void UYAimSensitivityControlSetting::NativeConstruct()
{
	Super::NativeConstruct();
	if (SensitivitySlider)
	{
		SensitivitySlider->OnValueChanged.AddDynamic(this, &UYAimSensitivityControlSetting::OnSensitivityChanged);
	}
}

void UYAimSensitivityControlSetting::LoadSetting()
{
	Super::LoadSetting();
	if (const UEnhancedInputLocalPlayerSubsystem* EISubsystem = GetOwningLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		UserSettings = EISubsystem->GetUserSettings<UExtendedEnhancedInputUserSettings>();
	}
	if (!UserSettings) return;
	const float CurrentYAimSensitivity = UserSettings->GetAimSensitivity().Y;
	SensitivitySlider->SetValue(CurrentYAimSensitivity);
	OnSensitivityChanged(CurrentYAimSensitivity);
}

void UYAimSensitivityControlSetting::SaveSetting()
{
	Super::SaveSetting();
	const float NewAimSensitivity = SensitivitySlider->GetValue();
	if (const UEnhancedInputLocalPlayerSubsystem* EISubsystem = GetOwningLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		UserSettings = EISubsystem->GetUserSettings<UExtendedEnhancedInputUserSettings>();
	}
	if (!UserSettings) return;
	const float X = UserSettings->GetAimSensitivity().X;
	UserSettings->SetAimSensitivity(FVector2d(X, NewAimSensitivity));
	LoadSetting();
}

void UYAimSensitivityControlSetting::ResetSetting()
{
	Super::ResetSetting();
}

void UYAimSensitivityControlSetting::OnSensitivityChanged(float Value)
{
	SensitivityText->SetText(FText::FromString(FString::SanitizeFloat(Value)));
}
