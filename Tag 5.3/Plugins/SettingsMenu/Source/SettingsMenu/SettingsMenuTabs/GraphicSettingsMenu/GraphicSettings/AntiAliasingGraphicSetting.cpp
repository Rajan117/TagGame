// Fill out your copyright notice in the Description page of Project Settings.


#include "AntiAliasingGraphicSetting.h"

#include "Components/ComboBoxString.h"
#include "GameFramework/GameUserSettings.h"

void UAntiAliasingGraphicSetting::NativeConstruct()
{
	Super::NativeConstruct();
	for (const auto& QualitySetting : QualitySettingsMap)
	{
		AntiAliasingQualityComboBox->AddOption(QualitySetting.Value);
	}
}

void UAntiAliasingGraphicSetting::LoadSetting()
{
	if (UserSettings)
	{
		AntiAliasingQualityComboBox->SetSelectedOption(QualitySettingsMap[UserSettings->GetAntiAliasingQuality()]);
	}
}

void UAntiAliasingGraphicSetting::SaveSetting()
{
	if (UserSettings)
	{
		UserSettings->SetAntiAliasingQuality(AntiAliasingQualityComboBox->GetSelectedIndex());
		UserSettings->ApplySettings(false);
	}
}
