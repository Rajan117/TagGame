// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "SettingsMenuTab.generated.h"

/**
 * Class that loads, saves and resets settings.
 * Will be subclassed by specific setting pages i.e. graphics.
 */
UCLASS()
class SETTINGSMENU_API USettingsMenuTab : public UUserWidget
{
	GENERATED_BODY()

protected:
	void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UButton* SaveButton;
	UPROPERTY(meta = (BindWidget))
	UButton* ResetButton;

	UFUNCTION()
	virtual void LoadSettings();
	UFUNCTION()
	virtual void SaveSettings();
	UFUNCTION()
	virtual void ResetSettings();
};
