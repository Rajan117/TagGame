// Fill out your copyright notice in the Description page of Project Settings.


#include "AudioSettingsTab.h"

#include "SettingsMenu/SettingsMenuTabs/Setting.h"

void UAudioSettingsTab::SaveSettings()
{
	for (const auto Setting : Settings)
	{
		Setting->SaveSetting();
	}
	
}

void UAudioSettingsTab::LoadSettings()
{
	for (const auto Setting : Settings)
	{
		Setting->LoadSetting();
	}
}

void UAudioSettingsTab::ResetSettings()
{
	for (const auto Setting : Settings)
	{
		Setting->ResetSetting();
	}
}
