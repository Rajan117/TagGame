// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphicSettingsTab.h"

#include "Components/ComboBoxString.h"
#include "GameFramework/GameUserSettings.h"

void UGraphicSettingsTab::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void UGraphicSettingsTab::LoadSettings()
{
	UserSettings = GEngine->GetGameUserSettings();
}

void UGraphicSettingsTab::SaveSettings()
{
	UserSettings->ApplySettings(false);
}

void UGraphicSettingsTab::ResetSettings()
{
	UserSettings->SetToDefaults();
}
