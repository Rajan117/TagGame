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
	if (UGameplayStatics::DoesSaveGameExist(TEXT("AudioSettingsSlot"), 0))
	{
		AudioSaveGameInstance = Cast<UAudioSettingsSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("AudioSettingsSlot"), 0));
		UKismetSystemLibrary::PrintString(this, TEXT("Audio Save Game Loaded"));
	}
	else
	{
		AudioSaveGameInstance = Cast<UAudioSettingsSaveGame>(UGameplayStatics::CreateSaveGameObject(UAudioSettingsSaveGame::StaticClass()));
		UKismetSystemLibrary::PrintString(this, TEXT("Audio Save Game Not Found"));
	}
	if (AudioSaveGameInstance)
	{
		
		if (AudioSaveGameInstance->VolumeSettings.Contains(VolumeNameText->GetText().ToString()))
		{
			UKismetSystemLibrary::PrintString(this, TEXT("Volume Setting Found"));
			VolumeSlider->SetValue(AudioSaveGameInstance->VolumeSettings[VolumeNameText->GetText().ToString()]);
		}
		else
		{
			UKismetSystemLibrary::PrintString(this, TEXT("Volume Setting Not Found"));
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
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Saving Volume Setting: %s = %f"), *VolumeNameText->GetText().ToString(), VolumeSlider->GetValue()));
		AudioSaveGameInstance->VolumeSettings.Add(VolumeNameText->GetText().ToString(), VolumeSlider->GetValue());
		UGameplayStatics::SaveGameToSlot(AudioSaveGameInstance, TEXT("AudioSettingsSlot"), 0);
		UKismetSystemLibrary::PrintString(this, TEXT("Audio Setting Saved"));
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
