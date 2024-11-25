// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsMenuSwitcher.generated.h"

class UButton;
class UWidgetSwitcher;
/**
 * 
 */
UCLASS()
class SETTINGSMENU_API USettingsMenuSwitcher : public UUserWidget
{
	GENERATED_BODY()
	
protected:
 	virtual void NativeConstruct() override;

	/*
	 * Widget switcher sets active settings tab.
	 * Index 0 = Graphics
	 * Index 1 = Gameplay
	 * Index 2 = Controls
	 * Index 3 = Keybinds
	 */
	UPROPERTY()
	UWidgetSwitcher* SettingsTabSwitcher;
	UPROPERTY()
	UButton* GraphicsTabButton;
	UPROPERTY()
	UButton* GameplayTabButton;
	UPROPERTY()
	UButton* ControlsTabButton;
	UPROPERTY()
	UButton* KeybindsTabButton;

	UFUNCTION()
	void OnGraphicsTabButtonClicked();
	UFUNCTION()
	void OnGameplayTabButtonClicked();
	UFUNCTION()
	void OnControlsTabButtonClicked();
	UFUNCTION()
	void OnKeybindsTabButtonClicked();
};
