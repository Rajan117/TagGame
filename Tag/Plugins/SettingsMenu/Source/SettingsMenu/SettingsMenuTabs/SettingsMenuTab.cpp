// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsMenuTab.h"

#include "Setting.h"
#include "Kismet/KismetSystemLibrary.h"

void USettingsMenuTab::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

void USettingsMenuTab::Init()
{
	UKismetSystemLibrary::PrintString(this, TEXT("Init Settings"));
	GetChildSettings(this);

	for (const auto Setting : Settings)
	{
		Setting->Init(this);
	}

	LoadSettings();
}

void USettingsMenuTab::GetChildSettings(UWidget* Widget)
{
	if (USetting* SettingWidget = Cast<USetting>(Widget))
	{
		UKismetSystemLibrary::PrintString(this, TEXT("Init Setting"));
		Settings.Add(SettingWidget);
		SettingWidget->Init(this);
	}
	else if (const UPanelWidget* Panel = Cast<UPanelWidget>(Widget))
	{
		for (int32 i = 0; i < Panel->GetChildrenCount(); ++i)
		{
			GetChildSettings(Panel->GetChildAt(i));
		}
	}
}

void USettingsMenuTab::LoadSettings()
{
	UKismetSystemLibrary::PrintString(this, TEXT("Loading Settings"));
	OnLoadSettingsDelegate.Broadcast();
}

void USettingsMenuTab::SaveSettings()
{
	UKismetSystemLibrary::PrintString(this, TEXT("Saving Settings"));
	OnSaveSettingsDelegate.Broadcast();
}

void USettingsMenuTab::ResetSettings()
{
	UKismetSystemLibrary::PrintString(this, TEXT("Reseting Settings"));
	OnResetSettingsDelegate.Broadcast();
}
