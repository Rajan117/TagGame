// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingsMenu/SettingsMenuTabs/ControlSettingsMenu/ControlSetting.h"
#include "InvertAimControlSetting.generated.h"

class UCheckBox;
/**
 * 
 */
UCLASS()
class SETTINGSMENU_API UInvertAimControlSetting : public UControlSetting
{
	GENERATED_BODY()

public:
	void LoadSetting() override;
	void SaveSetting() override;
	void ResetSetting() override;

private:
	UPROPERTY(meta = (BindWidget))
	UCheckBox* InvertAimCheckBox;

};
