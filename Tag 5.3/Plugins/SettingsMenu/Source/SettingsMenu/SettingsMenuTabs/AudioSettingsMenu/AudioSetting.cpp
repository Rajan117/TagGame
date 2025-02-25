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
	UKismetSystemLibrary::PrintString(this, TEXT("Loading Audio Setting"));
	if (AudioSaveGameInstance)
	{
		if (AudioSaveGameInstance->VolumeSettings.Contains(VolumeNameText->GetText().ToString()))
		{
			VolumeSlider->SetValue(AudioSaveGameInstance->VolumeSettings[VolumeNameText->GetText().ToString()]);
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
		AudioSaveGameInstance->VolumeSettings.Add(VolumeNameText->GetText().ToString(), VolumeSlider->GetValue());
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
	VolumeValueText->SetText(FText::FromString(FString::Printf(TEXT("%f"), (Value))));
}
