// Fill out your copyright notice in the Description page of Project Settings.


#include "MainSettingsMenu.h"

#include "Components/Button.h"

void UMainSettingsMenu::NativeConstruct()
{
	Super::NativeConstruct();
	if(BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UMainSettingsMenu::OnBackButtonClicked);
	}
}

void UMainSettingsMenu::InitSettingsMenu(UUserWidget* InPreviousWidget)
{
	PreviousWidget = InPreviousWidget;
}

void UMainSettingsMenu::OnBackButtonClicked()
{
	if(PreviousWidget) PreviousWidget->SetVisibility(ESlateVisibility::Visible);
	RemoveFromParent();
}
