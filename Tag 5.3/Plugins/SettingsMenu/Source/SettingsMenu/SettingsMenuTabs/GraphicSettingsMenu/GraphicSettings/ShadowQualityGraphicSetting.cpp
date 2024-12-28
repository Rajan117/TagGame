// Fill out your copyright notice in the Description page of Project Settings.


#include "ShadowQualityGraphicSetting.h"

#include "Components/ComboBoxString.h"
#include "GameFramework/GameUserSettings.h"

void UShadowQualityGraphicSetting::NativeConstruct()
{
	Super::NativeConstruct();
	for (const auto& QualitySetting : QualitySettingsMap)
	{
		ShadowQualityComboBox->AddOption(QualitySetting.Value);
	}
}

void UShadowQualityGraphicSetting::LoadSetting()
{
	if (UserSettings)
	{
		ShadowQualityComboBox->SetSelectedOption(QualitySettingsMap[UserSettings->GetShadowQuality()]);
	}
}

void UShadowQualityGraphicSetting::SaveSetting()
{
	if (UserSettings)
	{
		UserSettings->SetShadowQuality(ShadowQualityComboBox->GetSelectedIndex());
		UserSettings->ApplySettings(false);
	}
}
