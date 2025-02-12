// Fill out your copyright notice in the Description page of Project Settings.


#include "ExtendedEnhancedInputUserSettings.h"

void UExtendedEnhancedInputUserSettings::SetAimSensitivity(FVector2d AimSensitivity)
{
	AimSensitivityX = AimSensitivity.X;
	AimSensitivityY = AimSensitivity.Y;
}

FVector2d UExtendedEnhancedInputUserSettings::GetAimSensitivity() const
{
	return FVector2d(AimSensitivityX, AimSensitivityY);
}

FVector2d UExtendedEnhancedInputUserSettings::GetDefaultAimSensitivity() const
{
	return FVector2d(DefaultAimSensitivityX, DefaultAimSensitivityY);
}

void UExtendedEnhancedInputUserSettings::SetInvertAim(bool bInvertAimIn)
{
	bInvertAim = bInvertAimIn;
}

bool UExtendedEnhancedInputUserSettings::GetInvertAim() const
{
	return bInvertAim;
}
