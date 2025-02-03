// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingsMenu/SettingsMenuTabs/Setting.h"
#include "ControlSetting.generated.h"

class UExtendedEnhancedInputUserSettings;
/**
 * 
 */
UCLASS()
class SETTINGSMENU_API UControlSetting : public USetting
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY()
	UExtendedEnhancedInputUserSettings* UserSettings;
};
