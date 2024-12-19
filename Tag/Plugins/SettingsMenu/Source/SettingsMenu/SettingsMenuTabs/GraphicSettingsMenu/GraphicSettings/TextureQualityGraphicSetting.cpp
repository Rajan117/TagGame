// Fill out your copyright notice in the Description page of Project Settings.


#include "TextureQualityGraphicSetting.h"

#include "Components/ComboBoxString.h"
#include "GameFramework/GameUserSettings.h"

void UTextureQualityGraphicSetting::NativeConstruct()
{
	Super::NativeConstruct();

	for (const auto& QualitySetting : QualitySettingsMap)
	{
		TextureQualityComboBox->AddOption(QualitySetting.Value);
	}
}

void UTextureQualityGraphicSetting::LoadSetting()
{
	if (UserSettings)
	{
		TextureQualityComboBox->SetSelectedOption(QualitySettingsMap[UserSettings->GetTextureQuality()]);
	}
}

void UTextureQualityGraphicSetting::SaveSetting()
{
	if (UserSettings)
	{
		UserSettings->SetTextureQuality(TextureQualityComboBox->GetSelectedIndex());
		UserSettings->ApplySettings(false);
	}
}
