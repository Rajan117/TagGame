// Fill out your copyright notice in the Description page of Project Settings.


#include "ViewDistanceQualityGraphicSetting.h"

#include "Components/ComboBoxString.h"
#include "GameFramework/GameUserSettings.h"

void UViewDistanceQualityGraphicSetting::NativeConstruct()
{
	Super::NativeConstruct();
	for (const auto& QualitySetting : QualitySettingsMap)
	{
		ViewDistanceQualityComboBox->AddOption(QualitySetting.Value);
	}
}

void UViewDistanceQualityGraphicSetting::LoadSetting()
{
	if (UserSettings)
	{
		ViewDistanceQualityComboBox->SetSelectedOption(QualitySettingsMap[UserSettings->GetViewDistanceQuality()]);
	}
}

void UViewDistanceQualityGraphicSetting::SaveSetting()
{
	if (UserSettings)
	{
		UserSettings->SetViewDistanceQuality(ViewDistanceQualityComboBox->GetSelectedIndex());
		UserSettings->ApplySettings(false);
	}
}
