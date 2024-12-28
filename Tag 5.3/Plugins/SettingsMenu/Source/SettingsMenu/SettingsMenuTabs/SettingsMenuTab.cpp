// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsMenuTab.h"

#include "Setting.h"
#include "Components/VerticalBox.h"
#include "Kismet/KismetSystemLibrary.h"

void USettingsMenuTab::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

void USettingsMenuTab::Init()
{
	for (UWidget* Child : SettingsBox->GetAllChildren())
	{
		if (USetting* Setting = Cast<USetting>(Child))
		{
			Settings.Add(Setting);
		}
	}

	for (const auto Setting : Settings)
	{
		Setting->Init(this);
	}

	LoadSettings();
}

void USettingsMenuTab::LoadSettings()
{
	OnLoadSettingsDelegate.Broadcast();
}

void USettingsMenuTab::SaveSettings()
{
	OnSaveSettingsDelegate.Broadcast();
}

void USettingsMenuTab::ResetSettings()
{
	OnResetSettingsDelegate.Broadcast();
}
