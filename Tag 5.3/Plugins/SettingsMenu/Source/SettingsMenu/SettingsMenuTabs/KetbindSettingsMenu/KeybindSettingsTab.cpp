// Fill out your copyright notice in the Description page of Project Settings.


#include "KeybindSettingsTab.h"

#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"


void UKeybindSettingsTab::NativeConstruct()
{
	Super::NativeConstruct();
	if (const APlayerController* PC = Cast<APlayerController>(GetOwningPlayer()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			const UEnhancedInputLocalPlayerSubsystem* EISubsystem = GetOwningLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
			const UEnhancedInputUserSettings* UserSettings = EISubsystem->GetUserSettings();
			if (!UserSettings) return;
			for (const TPair<FGameplayTag, TObjectPtr<UEnhancedPlayerMappableKeyProfile>>& ProfilePair : UserSettings->GetAllSavedKeyProfiles())
			{
				const FGameplayTag& ProfileName = ProfilePair.Key;
				const TObjectPtr<UEnhancedPlayerMappableKeyProfile>& Profile = ProfilePair.Value;
	 
	 
				for (const TPair<FName, FKeyMappingRow>& RowPair : Profile->GetPlayerMappingRows())
				{
					// Use FKeyMappingRow to generate your UI!
				}
			}
		}
	}
}
