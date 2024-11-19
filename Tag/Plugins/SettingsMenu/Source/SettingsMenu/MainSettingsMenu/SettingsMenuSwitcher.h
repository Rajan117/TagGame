// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsMenuSwitcher.generated.h"

class UWidgetSwitcher;
/**
 * 
 */
UCLASS()
class SETTINGSMENU_API USettingsMenuSwitcher : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UWidgetSwitcher* WidgetSwitcher;
};
