// Fill out your copyright notice in the Description page of Project Settings.


#include "PostProcessingGraphicSetting.h"

#include "Components/ComboBoxString.h"
#include "GameFramework/GameUserSettings.h"

void UPostProcessingGraphicSetting::NativeConstruct()
{
	Super::NativeConstruct();
	for (const auto& QualitySetting : QualitySettingsMap)
	{
		PostProcessingQualityComboBox->AddOption(QualitySetting.Value);
	}
}

void UPostProcessingGraphicSetting::LoadSetting()
{
	if (UserSettings)
	{
		PostProcessingQualityComboBox->SetSelectedOption(QualitySettingsMap[UserSettings->GetPostProcessingQuality()]);
	}
}

void UPostProcessingGraphicSetting::SaveSetting()
{
	if (UserSettings)
	{
		UserSettings->SetPostProcessingQuality(PostProcessingQualityComboBox->GetSelectedIndex());
		UserSettings->ApplySettings(false);
	}
}
