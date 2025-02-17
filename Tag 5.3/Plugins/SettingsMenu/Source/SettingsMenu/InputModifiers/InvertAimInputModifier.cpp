// Fill out your copyright notice in the Description page of Project Settings.


#include "InvertAimInputModifier.h"

#include "EnhancedInputSubsystems.h"
#include "SettingsMenu/UserSettings/ExtendedEnhancedInputUserSettings.h"

FInputActionValue UInvertAimInputModifier::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime)
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
		const float Y = CurrentValue.Get<FVector>().Y * (Settings->GetInvertAim() ? -1.f : 1.f);
		return FVector(CurrentValue.Get<FVector>().X, Y, 0.f);
	}
	return CurrentValue;
}
