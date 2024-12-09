// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphicSetting.h"

#include "Kismet/KismetSystemLibrary.h"

void UGraphicSetting::NativeConstruct()
{
	Super::NativeConstruct();

	if (GEngine)
	{
		UserSettings = GEngine->GetGameUserSettings();
	}
}
