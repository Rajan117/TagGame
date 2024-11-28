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

	while (CurrentWidget)
	{
		ParentWidget = CurrentWidget;
		CurrentWidget = CurrentWidget->GetParent();
	}
	
	UKismetSystemLibrary::PrintString(this, ParentWidget ? ParentWidget->GetName() : TEXT("No Parent Widget"), true, false, FLinearColor::Red, 5.f);
	if (ParentWidget) ParentWidget->SetVisibility(ESlateVisibility::Hidden);
	if (UMainSettingsMenu* SettingsMenu = CreateWidget<UMainSettingsMenu>(GetWorld(), SettingsMenuClass))
	{
		SettingsMenu->AddToViewport();
		SettingsMenu->InitSettingsMenu(ParentWidget);
	}
}
