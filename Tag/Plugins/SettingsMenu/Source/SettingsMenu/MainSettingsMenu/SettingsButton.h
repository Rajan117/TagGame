// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsButton.generated.h"

class UMainSettingsMenu;
class UButton;
/**
 * 
 */
UCLASS()
class SETTINGSMENU_API USettingsButton : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* SettingsButton;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnSettingsButtonClicked();

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMainSettingsMenu> SettingsMenuClass;
};
