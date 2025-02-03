// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "ExtendedEnhancedInputUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class SETTINGSMENU_API UExtendedEnhancedInputUserSettings : public UEnhancedInputUserSettings
{
	GENERATED_BODY()

public:
    void SetAimSensitivity(FVector AimSensitivity);
	FVector GetAimSensitivity() const;
	FVector GetDefaultAimSensitivity() const;

	void SetInvertAim(bool bInvertAimIn);
	bool GetInvertAim() const;

protected:
	UPROPERTY(EditAnywhere)
	float AimSensitivityX = 1.f;
	UPROPERTY(EditAnywhere)
	float AimSensitivityY = 1.f;
	UPROPERTY()
	float DefaultAimSensitivityX = 1.f;
	UPROPERTY()
	float DefaultAimSensitivityY = 1.f;
	
	UPROPERTY(EditAnywhere)
	bool bInvertAim = false;
};
