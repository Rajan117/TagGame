// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "AudioSettingsSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SETTINGSMENU_API UAudioSettingsSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, Category = "Audio")
	TMap<FString, float> VolumeSettings;
	
};
