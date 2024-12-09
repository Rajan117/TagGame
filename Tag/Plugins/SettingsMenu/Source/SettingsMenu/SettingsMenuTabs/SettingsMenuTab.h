// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "SettingsMenuTab.generated.h"

class USetting;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoadSettings);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSaveSettings);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResetSettings);
/**
 * Class that loads, saves and resets settings.
 * Will be subclassed by specific setting pages i.e. graphics.
 */
UCLASS()
class SETTINGSMENU_API USettingsMenuTab : public UUserWidget
{
	GENERATED_BODY()

public:
	FOnLoadSettings OnLoadSettingsDelegate;
	FOnSaveSettings OnSaveSettingsDelegate;
	FOnSaveSettings OnResetSettingsDelegate;

	UFUNCTION()
	virtual void SaveSettings();
	UFUNCTION()
	virtual void ResetSettings();
	
protected:
	void NativeConstruct() override;

	virtual void Init();
	virtual void GetChildSettings(UWidget* Widget);
	TArray<USetting*> Settings;

	UFUNCTION()
	virtual void LoadSettings();
};
