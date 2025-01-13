// Fill out your copyright notice in the Description page of Project Settings.


#include "KeybindSettingsTab.h"

#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UserSettings/EnhancedInputUserSettings.h"

void UKeybindSettingsTab::LoadSettings()
{
	Super::LoadSettings();
	UKismetSystemLibrary::PrintString(this, TEXT("Loading keybinds..."), true, false, FLinearColor::Yellow, 5.f);
	const UEnhancedInputLocalPlayerSubsystem* EISubsystem = GetOwningLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	const UEnhancedInputUserSettings* UserSettings = EISubsystem->GetUserSettings();

	for (const TPair<FGameplayTag, TObjectPtr<UEnhancedPlayerMappableKeyProfile>>& ProfilePair : UserSettings->GetAllSavedKeyProfiles())
	{
		const FGameplayTag& ProfileName = ProfilePair.Key;
		const TObjectPtr<UEnhancedPlayerMappableKeyProfile>& Profile = ProfilePair.Value;
		for (const TPair<FName, FKeyMappingRow>& RowPair : Profile->GetPlayerMappingRows())
		{
			UKismetSystemLibrary::PrintString(this, RowPair.Key.ToString(), true, false, FLinearColor::Red, 5.f);
		}
	}
}
