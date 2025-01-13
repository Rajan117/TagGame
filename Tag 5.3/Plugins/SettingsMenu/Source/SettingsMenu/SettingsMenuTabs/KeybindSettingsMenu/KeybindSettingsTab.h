// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingsMenu/SettingsMenuTabs/SettingsMenuTab.h"
#include "KeybindSettingsTab.generated.h"

class UKeybindSetting;
/**
 * 
 */
UCLASS()
class SETTINGSMENU_API UKeybindSettingsTab : public USettingsMenuTab
{
	GENERATED_BODY()

public:
	virtual void LoadSettings() override;

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UKeybindSetting> KeybindSettingClass;
	
};
