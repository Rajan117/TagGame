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
	UPROPERTY(meta=(BindWidget))
	UWidgetSwitcher* SettingsTabSwitcher;
	UPROPERTY(meta=(BindWidget))
	UButton* GraphicsTabButton;
	UPROPERTY(meta=(BindWidget))
	UButton* GameplayTabButton;
	UPROPERTY(meta=(BindWidget))
	UButton* ControlsTabButton;
	UPROPERTY(meta=(BindWidget))
	UButton* KeybindsTabButton;
	UPROPERTY(meta = (BindWidget))
	UButton* AudioTabButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SaveButton;
	UPROPERTY(meta = (BindWidget))
	UButton* ResetButton;

	UFUNCTION()
	void OnGraphicsTabButtonClicked();
	UFUNCTION()
	void OnGameplayTabButtonClicked();
	UFUNCTION()
	void OnControlsTabButtonClicked();
	UFUNCTION()
	void OnKeybindsTabButtonClicked();
	UFUNCTION()
	void OnAudioTabButtonClicked();

	UFUNCTION()
	void OnSaveButtonClicked();
	UFUNCTION()
	void OnResetButtonClicked();
};
