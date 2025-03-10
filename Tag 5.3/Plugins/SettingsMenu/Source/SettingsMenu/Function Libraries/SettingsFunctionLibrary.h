// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SettingsFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SETTINGSMENU_API USettingsFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static void LoadAudioSettings();
	
};
