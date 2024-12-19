// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingsMenu/SettingsMenuTabs/GraphicSettingsMenu/GraphicSetting.h"
#include "ViewDistanceQualityGraphicSetting.generated.h"

class UComboBoxString;
/**
 * 
 */
UCLASS()
class SETTINGSMENU_API UViewDistanceQualityGraphicSetting : public UGraphicSetting
{
	GENERATED_BODY()
public:
	void NativeConstruct() override;
	void LoadSetting() override;
	void SaveSetting() override;

private:
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ViewDistanceQualityComboBox;
};
