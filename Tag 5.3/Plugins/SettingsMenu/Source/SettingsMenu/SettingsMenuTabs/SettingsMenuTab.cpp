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
