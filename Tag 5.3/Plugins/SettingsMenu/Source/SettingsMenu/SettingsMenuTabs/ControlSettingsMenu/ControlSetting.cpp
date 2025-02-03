// Fill out your copyright notice in the Description page of Project Settings.


#include "ControlSetting.h"

#include "EnhancedInputSubsystems.h"
#include "SettingsMenu/UserSettings/ExtendedEnhancedInputUserSettings.h"

void UControlSetting::NativeConstruct()
{
	Super::NativeConstruct();
	if (const UEnhancedInputLocalPlayerSubsystem* EISubsystem = GetOwningLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		UserSettings = EISubsystem->GetUserSettings<UExtendedEnhancedInputUserSettings>();
	}
}
