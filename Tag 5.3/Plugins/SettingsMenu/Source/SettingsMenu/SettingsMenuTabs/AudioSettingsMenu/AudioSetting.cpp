// Fill out your copyright notice in the Description page of Project Settings.


#include "AudioSetting.h"

#include "AudioSettingsSaveGame.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"

void UAudioSetting::NativeConstruct()
{
	Super::NativeConstruct();
	if (VolumeSlider)
	{
		VolumeSlider->OnValueChanged.AddDynamic(this, &UAudioSetting::OnVolumeSliderChanged);
	}
}

void UAudioSetting::LoadSetting()
{
	Super::LoadSetting();
	AudioSaveGameInstance = Cast<UAudioSettingsSaveGame>(UGameplayStatics::CreateSaveGameObject(UAudioSettingsSaveGame::StaticClass()));
	if (AudioSaveGameInstance)
	{
		if (AudioSaveGameInstance->VolumeSettings.Contains(VolumeName->GetText().ToString()))
		{
			VolumeSlider->SetValue(AudioSaveGameInstance->VolumeSettings[VolumeName->GetText().ToString()]);
		}
		else
		{
			VolumeSlider->SetValue(1.f);
		}
	}
}

void UAudioSetting::SaveSetting()
{
	Super::SaveSetting();
	if (SoundClass)
	{
		SoundClass->Properties.Volume = VolumeSlider->GetValue();
	}
	if (AudioSaveGameInstance)
	{
		AudioSaveGameInstance->VolumeSettings.Add(VolumeName->GetText().ToString(), VolumeSlider->GetValue());
		UGameplayStatics::SaveGameToSlot(AudioSaveGameInstance, TEXT("AudioSettingsSlot"), 0);
	}
}

void UAudioSetting::ResetSetting()
{
	Super::ResetSetting();
	VolumeSlider->SetValue(1.f);
}

void UAudioSetting::OnVolumeSliderChanged(float Value)
{
	VolumeText->SetText(FText::FromString(FString::Printf(TEXT("%f"), (Value))));
}
