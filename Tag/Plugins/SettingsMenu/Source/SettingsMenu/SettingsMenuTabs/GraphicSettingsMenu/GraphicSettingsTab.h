// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingsMenu/SettingsMenuTabs/SettingsMenuTab.h"
#include "GraphicSettingsTab.generated.h"

class UComboBoxString;
/**
 * 
 */
UCLASS()
class SETTINGSMENU_API UGraphicSettingsTab : public USettingsMenuTab
{
	GENERATED_BODY()

public:
	void LoadSettings() override;
	void SaveSettings() override;
	void ResetSettings() override;
	
protected:
	void NativeConstruct() override;

private:
	UPROPERTY()
	UGameUserSettings* UserSettings;
};
