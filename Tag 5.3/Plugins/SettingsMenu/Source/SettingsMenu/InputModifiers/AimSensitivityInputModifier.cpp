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
		return CurrentValue.Get<FVector>() * Settings->GetAimSensitivity();
	}
	return CurrentValue;
}
