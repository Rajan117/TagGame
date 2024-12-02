// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsMenuTab.h"

void USettingsMenuTab::NativeConstruct()
{
	Super::NativeConstruct();

	LoadSettings();

	if (SaveButton)
	{
		SaveButton->OnClicked.AddDynamic(this, &USettingsMenuTab::SaveSettings);
	}
	if (ResetButton)
	{
		ResetButton->OnClicked.AddDynamic(this, &USettingsMenuTab::ResetSettings);
	}
}

void USettingsMenuTab::LoadSettings()
{
}

void USettingsMenuTab::SaveSettings()
{
}

void USettingsMenuTab::ResetSettings()
{
}
