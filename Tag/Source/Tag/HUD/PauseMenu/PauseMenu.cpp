// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenu.h"

#include "Components/Button.h"
#include "Tag/Controller/TagPlayerController.h"

void UPauseMenu::NativeConstruct()
{
	Super::NativeConstruct();
	if (ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &ThisClass::UPauseMenu::OnResumeButtonPressed);
	}
}

void UPauseMenu::OnResumeButtonPressed()
{
	if (ATagPlayerController* TagPlayerController = Cast<ATagPlayerController>(GetOwningPlayer()))
	{
		TagPlayerController->HidePauseMenu();
	}
}
