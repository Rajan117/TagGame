// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingsMenu/SettingsMenuTabs/Setting.h"
#include "GraphicSetting.generated.h"

/**
 * 
 */
UCLASS()
class SETTINGSMENU_API UGraphicSetting : public USetting
{
	GENERATED_BODY()

public:
	void ResetSetting() override;
	
protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY()
	UGameUserSettings* UserSettings;

	TMap<int32, FString> QualitySettingsMap = {
		{0, TEXT("Low")},
		{1, TEXT("Medium")},
		{2, TEXT("High")},
		{3, TEXT("Epic")}
	};
};
