// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingsMenu/SettingsMenuTabs/GraphicSettingsMenu/GraphicSetting.h"
#include "VSyncGraphicSetting.generated.h"

class UCheckBox;
/**
 * 
 */
UCLASS()
class SETTINGSMENU_API UVSyncGraphicSetting : public UGraphicSetting
{
	GENERATED_BODY()
public:
	void LoadSetting() override;
	void SaveSetting() override;

private:
	UPROPERTY(meta = (BindWidget))
	UCheckBox* VSyncCheckBox;
};
