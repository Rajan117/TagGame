// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingsMenu/SettingsMenuTabs/SettingsMenuTab.h"
#include "KeybindSettingsTab.generated.h"

class UInputMappingContext;
/**
 * 
 */
UCLASS()
class SETTINGSMENU_API UKeybindSettingsTab : public USettingsMenuTab
{
	GENERATED_BODY()

protected:
	void NativeConstruct() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input | Mapping Contexts")
	UInputMappingContext* InputMappingContext;
	
};
