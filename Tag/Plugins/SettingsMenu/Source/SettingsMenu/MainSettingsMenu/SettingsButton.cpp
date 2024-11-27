// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsButton.h"

#include "MainSettingsMenu.h"
#include "Components/Button.h"

void USettingsButton::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (SettingsButton)
	{
		SettingsButton->OnClicked.AddDynamic(this, &USettingsButton::OnSettingsButtonClicked);
	}
}

void USettingsButton::OnSettingsButtonClicked()
{
	ParentWidget = Cast<UUserWidget>(GetRootWidget());
	if (ParentWidget) ParentWidget->SetVisibility(ESlateVisibility::Hidden);
	if (UMainSettingsMenu* SettingsMenu = CreateWidget<UMainSettingsMenu>(GetWorld(), SettingsMenuClass))
	{
		SettingsMenu->AddToViewport();
		SettingsMenu->InitSettingsMenu(ParentWidget);
	}
}
