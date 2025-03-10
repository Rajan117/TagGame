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
	
	virtual void LoadSetting(UAudioSettingsSaveGame* AudioSettingsSaveGame);
	virtual void SaveSetting(UAudioSettingsSaveGame* AudioSettingsSaveGame);
	virtual void ResetSetting() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Class")
	USoundClass* SoundClass;
	
private:
	UPROPERTY(meta = (BindWidget))
	USlider* VolumeSlider;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* VolumeValueText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* VolumeNameText;

	UFUNCTION()
	void OnVolumeSliderChanged(float Value);

	UPROPERTY()
	UAudioSettingsSaveGame* AudioSaveGameInstance;
};
