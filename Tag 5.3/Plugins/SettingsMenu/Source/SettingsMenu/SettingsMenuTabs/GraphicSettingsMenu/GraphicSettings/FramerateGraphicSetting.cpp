// Fill out your copyright notice in the Description page of Project Settings.


#include "FramerateGraphicSetting.h"

#include "Components/ComboBoxString.h"
#include "GameFramework/GameUserSettings.h"

void UFramerateGraphicSetting::LoadSetting()
{
	if (!UserSettings) return;
	FramerateComboBox->SetSelectedOption(FString::SanitizeFloat(UserSettings->GetFrameRateLimit()));
}

void UFramerateGraphicSetting::SaveSetting()
{
	if (!UserSettings) return;
	UserSettings->SetFrameRateLimit(FCString::Atof(*FramerateComboBox->GetSelectedOption()));
	UserSettings->ApplySettings(false);
}
