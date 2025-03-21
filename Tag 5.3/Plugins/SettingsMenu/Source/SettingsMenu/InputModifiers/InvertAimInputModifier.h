// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputModifiers.h"
#include "InvertAimInputModifier.generated.h"

class UExtendedEnhancedInputUserSettings;
/**
 * 
 */
UCLASS()
class SETTINGSMENU_API UInvertAimInputModifier : public UInputModifier
{
	GENERATED_BODY()

protected:
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;
 
	UPROPERTY(Transient)
	UExtendedEnhancedInputUserSettings* Settings;
};
