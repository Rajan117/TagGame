// Fill out your copyright notice in the Description page of Project Settings.


#include "WindowModeGraphicSetting.h"

#include "Components/ComboBoxString.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"

void UWindowModeGraphicSetting::LoadSetting()
{
	if (UserSettings)
	{
		UKismetSystemLibrary::PrintString(this, TEXT("Loading Window Mode"));

		switch (UserSettings->GetFullscreenMode())
		{
		case EWindowMode::Fullscreen:
			WindowModeComboBox->SetSelectedOption(TEXT("Fullscreen"));
			break;
		case EWindowMode::WindowedFullscreen:
			WindowModeComboBox->SetSelectedOption(TEXT("Windowed Fullscreen"));
			break;
		case EWindowMode::Windowed:
			WindowModeComboBox->SetSelectedOption(TEXT("Windowed"));
			break;
		default:
			WindowModeComboBox->SetSelectedOption(TEXT("Windowed"));
			break;
		}
	}
}

void UWindowModeGraphicSetting::SaveSetting()
{
	if (UserSettings)
	{
		UKismetSystemLibrary::PrintString(this, TEXT("Saving Window Mode"));

		const FString SelectedOption = WindowModeComboBox->GetSelectedOption();
		if (SelectedOption == TEXT("Fullscreen"))
		{
			UserSettings->SetFullscreenMode(EWindowMode::Fullscreen);
		}
		else if (SelectedOption == TEXT("Windowed Fullscreen"))
		{
			UserSettings->SetFullscreenMode(EWindowMode::WindowedFullscreen);
		}
		else if (SelectedOption == TEXT("Windowed"))
		{
			UserSettings->SetFullscreenMode(EWindowMode::Windowed);
		}
		else
		{
			UserSettings->SetFullscreenMode(EWindowMode::Windowed);
		}
	}
}
