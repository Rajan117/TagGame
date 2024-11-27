// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainSettingsMenu.generated.h"

class UButton;
class USettingsMenuSwitcher;
/**
 * 
 */
UCLASS()
class SETTINGSMENU_API UMainSettingsMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	void InitSettingsMenu(UUserWidget* InPreviousWidget);
	
protected:
	UPROPERTY(meta=(BindWidget))
	USettingsMenuSwitcher* SettingsMenuSwitcher;
	UPROPERTY(meta=(BindWidget))
	UButton* BackButton;

	UFUNCTION()
	void OnBackButtonClicked();

private:
	UPROPERTY()
	UUserWidget* PreviousWidget;
};
