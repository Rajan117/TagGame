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
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("VolumeNameText: %s"), *VolumeNameText->GetText().ToString()));
		VolumeSlider->SetValue(AudioSettingsSaveGame->VolumeSettings[VolumeNameText->GetText().ToString()]*100.f);
		VolumeValueText->SetText(FText::FromString(FString::Printf(TEXT("%f"), AudioSettingsSaveGame->VolumeSettings[VolumeNameText->GetText().ToString()]*100.f)));
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
	if (SoundClass)	SoundClass->Properties.Volume = VolumeSlider->GetValue();
	AudioSettingsSaveGame->VolumeSettings.Add(VolumeNameText->GetText().ToString(), VolumeSlider->GetValue()/100.f);
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
