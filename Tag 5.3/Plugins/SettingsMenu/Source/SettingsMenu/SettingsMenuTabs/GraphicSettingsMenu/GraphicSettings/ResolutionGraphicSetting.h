// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingsMenu/SettingsMenuTabs/GraphicSettingsMenu/GraphicSetting.h"
#include "ResolutionGraphicSetting.generated.h"

/**
 * 
 */
UCLASS()
class SETTINGSMENU_API UResolutionGraphicSetting : public UGraphicSetting
{
	GENERATED_BODY()

public:
	void LoadSetting() override;
	void SaveSetting() override;

private:
	void PopulateResolutionComboBox();
	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* ResolutionComboBox;
	
};
