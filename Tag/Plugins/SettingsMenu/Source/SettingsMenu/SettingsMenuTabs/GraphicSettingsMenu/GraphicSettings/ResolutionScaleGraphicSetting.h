// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingsMenu/SettingsMenuTabs/GraphicSettingsMenu/GraphicSetting.h"
#include "ResolutionScaleGraphicSetting.generated.h"

class UTextBlock;
class USlider;
/**
 * 
 */
UCLASS()
class SETTINGSMENU_API UResolutionScaleGraphicSetting : public UGraphicSetting
{
	GENERATED_BODY()

public:
	void LoadSetting() override;
	void SaveSetting() override;

private:
	UPROPERTY(meta = (BindWidget))
	USlider* ResolutionScaleSlider;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ResolutionScaleText;

	UFUNCTION()
	void OnResolutionScaleValueChanged(float Value);
};
