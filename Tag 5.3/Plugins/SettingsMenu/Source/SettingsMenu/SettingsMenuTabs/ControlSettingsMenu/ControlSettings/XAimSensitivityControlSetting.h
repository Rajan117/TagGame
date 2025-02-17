// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingsMenu/SettingsMenuTabs/ControlSettingsMenu/ControlSetting.h"
#include "XAimSensitivityControlSetting.generated.h"

class UTextBlock;
class USlider;
/**
 * 
 */
UCLASS()
class SETTINGSMENU_API UXAimSensitivityControlSetting : public UControlSetting
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	void LoadSetting() override;
	void SaveSetting() override;
	void ResetSetting() override;

private:
	UPROPERTY(meta = (BindWidget))
	USlider* SensitivitySlider;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SensitivityText;

	UFUNCTION()
	void OnSensitivityChanged(float Value);
};
