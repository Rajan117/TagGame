// Fill out your copyright notice in the Description page of Project Settings.


#include "AimSensitivityInputModifier.h"

#include "EnhancedInputSubsystems.h"
#include "SettingsMenu/UserSettings/ExtendedEnhancedInputUserSettings.h"

FInputActionValue UAimSensitivityInputModifier::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime)
{
	if(!Settings)
	{
		Settings = PlayerInput->GetOwningLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()->GetUserSettings<UExtendedEnhancedInputUserSettings>();
	}
	
	return CurrentValue.Get<FVector>() * Settings->GetAimSensitivity();
}
