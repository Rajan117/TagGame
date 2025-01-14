// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingsMenu/SettingsMenuTabs/SettingsMenuTab.h"
#include "KeybindSettingsTab.generated.h"

class UEnhancedInputUserSettings;
class UVerticalBox;

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
	virtual void SaveSettings() override;
	virtual void ResetSettings() override;

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UKeybindSetting> KeybindSettingClass;

private:
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* KeybindSettingsBox;

	UPROPERTY()
	UEnhancedInputUserSettings* UserSettings;
	
};
