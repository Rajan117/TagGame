// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsMenuTab.h"

#include "Setting.h"
#include "Kismet/KismetSystemLibrary.h"

void USettingsMenuTab::NativeConstruct()
{
	Super::NativeConstruct();

	Init();

	if (SaveButton)
	{
		SaveButton->OnClicked.AddDynamic(this, &USettingsMenuTab::SaveSettings);
	}
	if (ResetButton)
	{
		ResetButton->OnClicked.AddDynamic(this, &USettingsMenuTab::ResetSettings);
	}
}

void USettingsMenuTab::Init()
{
	if (UWidget* RootWidget = GetRootWidget())
	{
		GetChildSettings(RootWidget);
	}

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
