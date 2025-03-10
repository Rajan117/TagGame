// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsFunctionLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "SettingsMenu/SettingsMenuTabs/AudioSettingsMenu/AudioSettingsSaveGame.h"
#include "Sound/SoundClass.h"

void USettingsFunctionLibrary::LoadAudioSettings()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("AudioSettingsSlot"), 0))
	{
		if (UAudioSettingsSaveGame* AudioSaveGameInstance = Cast<UAudioSettingsSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("AudioSettingsSlot"), 0)))
		{
			for (auto& VolumeSetting : AudioSaveGameInstance->VolumeSettings)
			{
				if (USoundClass* SoundClass = LoadObject<USoundClass>(nullptr, *VolumeSetting.Key))
				{
					SoundClass->Properties.Volume = VolumeSetting.Value;
				}
			}
		}
	}
}
