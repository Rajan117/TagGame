// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingsMenu/SettingsMenuTabs/SettingsMenuTab.h"
#include "AudioSettingsTab.generated.h"

class UAudioSettingsSaveGame;
/**
 * 
 */
UCLASS()
class SETTINGSMENU_API UAudioSettingsTab : public USettingsMenuTab
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
	virtual void SaveSettings() override;
	virtual void LoadSettings() override;

protected:
	UPROPERTY()
	UAudioSettingsSaveGame* AudioSaveGameInstance;
};
