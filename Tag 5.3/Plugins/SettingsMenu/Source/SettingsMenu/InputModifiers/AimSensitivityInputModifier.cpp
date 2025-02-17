// Fill out your copyright notice in the Description page of Project Settings.


#include "AimSensitivityInputModifier.h"

#include "EnhancedInputSubsystems.h"
#include "SettingsMenu/UserSettings/ExtendedEnhancedInputUserSettings.h"

FInputActionValue UAimSensitivityInputModifier::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime)
{
	if(!Settings)
	{
		if(PlayerInput && PlayerInput->GetOwningLocalPlayer())
		{
			if (const UEnhancedInputLocalPlayerSubsystem* EISubsystem = PlayerInput->GetOwningLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				Settings = EISubsystem->GetUserSettings<UExtendedEnhancedInputUserSettings>();
			}
		}
	}
	if (Settings)
	{
		const float XSensitivity = CurrentValue.Get<FVector2d>().X * Settings->GetAimSensitivity().X;
		const float YSensitivity = CurrentValue.Get<FVector2d>().Y * Settings->GetAimSensitivity().Y;
		return FVector2d(XSensitivity, YSensitivity);
	}
	return CurrentValue;
}
