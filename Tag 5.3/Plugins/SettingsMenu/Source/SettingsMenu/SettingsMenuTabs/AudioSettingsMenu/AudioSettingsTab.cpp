// Fill out your copyright notice in the Description page of Project Settings.


#include "AudioSettingsTab.h"

#include "AudioSetting.h"
#include "AudioSettingsSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "SettingsMenu/SettingsMenuTabs/Setting.h"

void UAudioSettingsTab::NativeConstruct()
{
	Super::NativeConstruct();
	if (UGameplayStatics::DoesSaveGameExist(TEXT("AudioSettingsSlot"), 0))
	{
		AudioSaveGameInstance = Cast<UAudioSettingsSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("AudioSettingsSlot"), 0));
	}	
	else
	{
		AudioSaveGameInstance = Cast<UAudioSettingsSaveGame>(UGameplayStatics::CreateSaveGameObject(UAudioSettingsSaveGame::StaticClass()));
	}
}

void UAudioSettingsTab::SaveSettings()
{
	for (const auto Setting : Settings)
	{
		if (UAudioSetting* AudioSetting = Cast<UAudioSetting>(Setting))
		{
			AudioSetting->SaveSetting(AudioSaveGameInstance);
		}
	}

	UGameplayStatics::SaveGameToSlot(AudioSaveGameInstance, TEXT("AudioSettingsSlot"), 0);
	LoadSettings();
}

void UAudioSettingsTab::LoadSettings()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("AudioSettingsSlot"), 0))
	{
		AudioSaveGameInstance = Cast<UAudioSettingsSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("AudioSettingsSlot"), 0));
	}	
	else
	{
		AudioSaveGameInstance = Cast<UAudioSettingsSaveGame>(UGameplayStatics::CreateSaveGameObject(UAudioSettingsSaveGame::StaticClass()));
	}
	for (const auto Setting : Settings)
	{
		if (UAudioSetting* AudioSetting = Cast<UAudioSetting>(Setting))
		{
			AudioSetting->LoadSetting(AudioSaveGameInstance);
		}
	}
}

