// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GraphicSetting.generated.h"

/**
 * 
 */
UCLASS()
class SETTINGSMENU_API UGraphicSetting : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(UGameUserSettings* InUserSettings);
	virtual void LoadSetting();
	virtual void SaveSetting();

protected:
	UPROPERTY()
	UGameUserSettings* UserSettings;
};
