// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsMenuSwitcher.h"

#include "UnrealWidgetFwd.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"


void USettingsMenuSwitcher::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (GraphicsTabButton)
	{
		GraphicsTabButton->OnClicked.AddDynamic(this, &USettingsMenuSwitcher::OnGraphicsTabButtonClicked);
	}
	if (GameplayTabButton)
	{
		GameplayTabButton->OnClicked.AddDynamic(this, &USettingsMenuSwitcher::OnGameplayTabButtonClicked);
	}
	if (ControlsTabButton)
	{
		ControlsTabButton->OnClicked.AddDynamic(this, &USettingsMenuSwitcher::OnControlsTabButtonClicked);
	}
	if (KeybindsTabButton)
	{
		KeybindsTabButton->OnClicked.AddDynamic(this, &USettingsMenuSwitcher::OnKeybindsTabButtonClicked);
	}
}

void USettingsMenuSwitcher::OnGraphicsTabButtonClicked()
{
	SettingsTabSwitcher->SetActiveWidgetIndex(0);
}

void USettingsMenuSwitcher::OnGameplayTabButtonClicked()
{
	SettingsTabSwitcher->SetActiveWidgetIndex(1);
}

void USettingsMenuSwitcher::OnControlsTabButtonClicked()
{
	SettingsTabSwitcher->SetActiveWidgetIndex(2);
}

void USettingsMenuSwitcher::OnKeybindsTabButtonClicked()
{
	SettingsTabSwitcher->SetActiveWidgetIndex(3);
}
