// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingsMenu/SettingsMenuTabs/Setting.h"
#include "AudioSetting.generated.h"

class UAudioSettingsSaveGame;
class USlider;
/**
 * 
 */
UCLASS()
class SETTINGSMENU_API UAudioSetting : public USetting
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	virtual void LoadSetting() override;
	virtual void SaveSetting() override;
	virtual void ResetSetting() override;

protected:
	UPROPERTY()
	USoundClass* SoundClass;
	
private:
	UPROPERTY(meta = (BindWidget))
	USlider* VolumeSlider;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* VolumeText;
	UPROPERTY()
	UTextBlock* VolumeName;

	UFUNCTION()
	void OnVolumeSliderChanged(float Value);

	UPROPERTY()
	UAudioSettingsSaveGame* AudioSaveGameInstance;
};
