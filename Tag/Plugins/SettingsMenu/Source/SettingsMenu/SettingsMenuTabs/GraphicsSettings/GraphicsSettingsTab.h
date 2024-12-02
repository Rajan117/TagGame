// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingsMenu/SettingsMenuTabs/SettingsMenuTab.h"
#include "GraphicsSettingsTab.generated.h"

/**
 * 
 */
UCLASS()
class SETTINGSMENU_API UGraphicsSettingsTab : public USettingsMenuTab
{
	GENERATED_BODY()

protected:
	void NativeConstruct() override;

	void LoadSettings() override;
	void SaveSettings() override;
	void ResetSettings() override;
	
};
