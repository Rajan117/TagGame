// Fill out your copyright notice in the Description page of Project Settings.


#include "AudioSetting.h"

#include "AudioSettingsSaveGame.h"
#include "BaseGizmos/GizmoMath.h"
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

void UAudioSetting::LoadSetting(UAudioSettingsSaveGame* AudioSettingsSaveGame)
{
	if (!AudioSettingsSaveGame) return;
		
	if (AudioSettingsSaveGame->VolumeSettings.Contains(VolumeNameText->GetText().ToString()))
	{
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("%s: %f"), *VolumeNameText->GetText().ToString(), AudioSettingsSaveGame->VolumeSettings[VolumeNameText->GetText().ToString()]));
		const float LoadedVolume = AudioSettingsSaveGame->VolumeSettings[VolumeNameText->GetText().ToString()];
		const int32 RoundedVolume = FMath::RoundToInt(LoadedVolume*100.f);
		VolumeSlider->SetValue(RoundedVolume);
		VolumeValueText->SetText(FText::FromString(FString::Printf(TEXT("%i"), RoundedVolume)));
	}
	else
	{
		VolumeSlider->SetValue(100.f);
		VolumeSlider->SetIsEnabled(false);
	}
}

void UAudioSetting::SaveSetting(UAudioSettingsSaveGame* AudioSettingsSaveGame)
{
	if (!AudioSettingsSaveGame) return;
	const float NewVolume = VolumeSlider->GetValue()/100.f;
	if (SoundClass)	SoundClass->Properties.Volume = NewVolume;
	AudioSettingsSaveGame->VolumeSettings.Add(VolumeNameText->GetText().ToString(), NewVolume);
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("%s: %f"), *VolumeNameText->GetText().ToString(), AudioSettingsSaveGame->VolumeSettings[VolumeNameText->GetText().ToString()]));
}

void UAudioSetting::ResetSetting()
{
	Super::ResetSetting();
	VolumeSlider->SetValue(100.f);
}

void UAudioSetting::OnVolumeSliderChanged(float Value)
{
	const int32 RoundedValue = FMath::RoundToInt(Value);
	VolumeSlider->SetValue(RoundedValue);
	VolumeValueText->SetText(FText::FromString(FString::Printf(TEXT("%i"), (RoundedValue))));
}
