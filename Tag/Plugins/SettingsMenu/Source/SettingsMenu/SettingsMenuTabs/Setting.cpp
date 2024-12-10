// Fill out your copyright notice in the Description page of Project Settings.


#include "Setting.h"

#include "SettingsMenuTab.h"
#include "Kismet/KismetSystemLibrary.h"

void USetting::NativeConstruct()
{
	Super::NativeConstruct();
}

void USetting::Init(USettingsMenuTab* InSettingsMenuTab)
{
	SettingsMenuTab = InSettingsMenuTab;
	if (SettingsMenuTab)
	{
		SettingsMenuTab->OnLoadSettingsDelegate.AddDynamic(this, &USetting::LoadSetting);
		SettingsMenuTab->OnSaveSettingsDelegate.AddDynamic(this, &USetting::SaveSetting);
		SettingsMenuTab->OnResetSettingsDelegate.AddDynamic(this, &USetting::ResetSetting);
	}
}

void USetting::LoadSetting()
{
}

void USetting::SaveSetting()
{
}

void USetting::ResetSetting()
{
}
