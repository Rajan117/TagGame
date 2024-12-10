// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsButton.h"

#include "MainSettingsMenu.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

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
	UWidget* CurrentWidget = this;
	UWidget* ParentWidget = nullptr;
	uint8 Iteration = 0;
	while (CurrentWidget && Iteration++ < 1000)
	{
		ParentWidget = CurrentWidget;
		CurrentWidget = CurrentWidget->GetParent();
	}
	
	if (ParentWidget) ParentWidget->SetVisibility(ESlateVisibility::Hidden);
	if (UMainSettingsMenu* SettingsMenu = CreateWidget<UMainSettingsMenu>(GetWorld(), SettingsMenuClass))
	{
		SettingsMenu->AddToViewport();
		SettingsMenu->InitSettingsMenu(ParentWidget);
	}
}
