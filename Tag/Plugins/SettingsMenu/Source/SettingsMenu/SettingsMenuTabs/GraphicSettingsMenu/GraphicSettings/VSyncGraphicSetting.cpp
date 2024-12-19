// Fill out your copyright notice in the Description page of Project Settings.


#include "VSyncGraphicSetting.h"

#include "Components/CheckBox.h"
#include "GameFramework/GameUserSettings.h"

void UVSyncGraphicSetting::LoadSetting()
{
	if (UserSettings)
	{
		VSyncCheckBox->SetIsChecked(UserSettings->IsVSyncEnabled());
	}
}

void UVSyncGraphicSetting::SaveSetting()
{
	if (UserSettings)
	{
		UserSettings->SetVSyncEnabled(VSyncCheckBox->IsChecked());
		UserSettings->ApplySettings(false);
	}
}
