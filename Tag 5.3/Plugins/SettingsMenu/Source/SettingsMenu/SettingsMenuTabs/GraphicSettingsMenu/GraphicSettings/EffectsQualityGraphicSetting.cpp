// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectsQualityGraphicSetting.h"

#include "Components/ComboBoxString.h"
#include "GameFramework/GameUserSettings.h"

void UEffectsQualityGraphicSetting::NativeConstruct()
{
	Super::NativeConstruct();
	for (const auto& QualitySetting : QualitySettingsMap)
	{
		EffectsQualityComboBox->AddOption(QualitySetting.Value);
	}
}

void UEffectsQualityGraphicSetting::LoadSetting()
{
	if (UserSettings)
	{
		EffectsQualityComboBox->SetSelectedOption(QualitySettingsMap[UserSettings->GetVisualEffectQuality()]);
	}
}

void UEffectsQualityGraphicSetting::SaveSetting()
{
	if (UserSettings)
	{
		UserSettings->SetVisualEffectQuality(EffectsQualityComboBox->GetSelectedIndex());
		UserSettings->ApplySettings(false);
	}
}
