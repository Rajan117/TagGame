// Fill out your copyright notice in the Description page of Project Settings.


#include "AimSensitivityControlSetting.h"

#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "SettingsMenu/UserSettings/ExtendedEnhancedInputUserSettings.h"

void UAimSensitivityControlSetting::NativeConstruct()
{
	Super::NativeConstruct();
	if (SensitivitySlider)
	{
		SensitivitySlider->OnValueChanged.AddDynamic(this, &UAimSensitivityControlSetting::OnSensitivityChanged);
	}
}

void UAimSensitivityControlSetting::LoadSetting()
{
	Super::LoadSetting();
	const FVector CurrentAimSensitivity = UserSettings->GetAimSensitivity();
	SensitivitySlider->SetValue(CurrentAimSensitivity.X);
	OnSensitivityChanged(CurrentAimSensitivity.X);
}

void UAimSensitivityControlSetting::SaveSetting()
{
	Super::SaveSetting();
	const FVector NewAimSensitivity = FVector(SensitivitySlider->GetValue());
	UserSettings->SetAimSensitivity(NewAimSensitivity);
	LoadSetting();
}

void UAimSensitivityControlSetting::ResetSetting()
{
	Super::ResetSetting();
	UserSettings->SetAimSensitivity(UserSettings->GetDefaultAimSensitivity());
	LoadSetting();
}

void UAimSensitivityControlSetting::OnSensitivityChanged(float Value)
{
	SensitivityText->SetText(FText::FromString(FString::SanitizeFloat(Value)));
}
