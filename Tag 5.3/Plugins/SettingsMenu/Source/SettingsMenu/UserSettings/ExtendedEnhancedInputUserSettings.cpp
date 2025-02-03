// Fill out your copyright notice in the Description page of Project Settings.


#include "ExtendedEnhancedInputUserSettings.h"

void UExtendedEnhancedInputUserSettings::SetAimSensitivity(FVector AimSensitivity)
{
	AimSensitivityX = AimSensitivity.X;
	AimSensitivityY = AimSensitivity.Y;
}

FVector UExtendedEnhancedInputUserSettings::GetAimSensitivity() const
{
	return FVector(AimSensitivityX, AimSensitivityY, 0);
}

void UExtendedEnhancedInputUserSettings::SetInvertAim(bool bInvertAim)
{
	bInvertAim = bInvertAim;
}

bool UExtendedEnhancedInputUserSettings::GetInvertAim() const
{
	return bInvertAim;
}
