// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphicSetting.h"

#include "GameFramework/GameUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"

void UGraphicSetting::ResetSetting()
{
	if (UserSettings)
	{
		UserSettings->SetToDefaults();
	}
}

void UGraphicSetting::NativeConstruct()
{
	Super::NativeConstruct();

	if (GEngine)
	{
		UserSettings = GEngine->GetGameUserSettings();
	}
}
