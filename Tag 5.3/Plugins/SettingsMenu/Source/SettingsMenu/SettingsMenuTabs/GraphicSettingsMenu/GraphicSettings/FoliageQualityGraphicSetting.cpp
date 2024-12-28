// Fill out your copyright notice in the Description page of Project Settings.


#include "FoliageQualityGraphicSetting.h"

#include "Components/ComboBoxString.h"
#include "GameFramework/GameUserSettings.h"

void UFoliageQualityGraphicSetting::NativeConstruct()
{
	Super::NativeConstruct();
	for (const auto& QualitySetting : QualitySettingsMap)
	{
		FoliageQualityComboBox->AddOption(QualitySetting.Value);
	}
}

void UFoliageQualityGraphicSetting::LoadSetting()
{
	if (UserSettings)
	{
		FoliageQualityComboBox->SetSelectedOption(QualitySettingsMap[UserSettings->GetFoliageQuality()]);
	}
}

void UFoliageQualityGraphicSetting::SaveSetting()
{
	if (UserSettings)
	{
		UserSettings->SetFoliageQuality(FoliageQualityComboBox->GetSelectedIndex());
		UserSettings->ApplySettings(false);
	}
}
