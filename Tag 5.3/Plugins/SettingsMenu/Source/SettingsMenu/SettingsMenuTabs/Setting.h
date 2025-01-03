// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Setting.generated.h"

class USettingsMenuTab;
/**
 * 
 */
UCLASS()
class SETTINGSMENU_API USetting : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void Init(USettingsMenuTab* InSettingsMenuTab);
	UFUNCTION()
	virtual void LoadSetting();
	UFUNCTION()
	virtual void SaveSetting();
	UFUNCTION()
	virtual void ResetSetting();

protected:
	UPROPERTY()
	USettingsMenuTab* SettingsMenuTab;
};
