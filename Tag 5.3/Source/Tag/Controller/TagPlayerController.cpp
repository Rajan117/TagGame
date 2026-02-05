// Fill out your copyright notice in the Description page of Project Settings.


#include "TagPlayerController.h"
#include "Tag/Character/TagCharacter.h"
#include "Tag/HUD/Scoreboard/Scoreboard.h"

#include "EnhancedInputComponent.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Tag/HUD/PauseMenu/PauseMenu.h"


void ATagPlayerController::BeginPlay()
{
	Super::BeginPlay();

	const FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);
	SetShowMouseCursor(false);
}

void ATagPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ATagPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();
}

void ATagPlayerController::ShowScoreboard()
{
	if (ScoreboardRef)
	{
		HideScoreboard();
		return;
	}
	if (ScoreboardClass)
	{
		ScoreboardRef = CreateWidget<UScoreboard>(this, ScoreboardClass);
		if (ScoreboardRef)
		{
			ScoreboardRef->AddToViewport();
		}
	}
}

void ATagPlayerController::HideScoreboard()
{
	if (ScoreboardRef)
	{
		ScoreboardRef->RemoveFromParent();
		ScoreboardRef = nullptr;
	}
}

void ATagPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);

	if (ATagCharacter* TagCharacter = Cast<ATagCharacter>(P))
	{
		TagCharacter->GetAbilitySystemComponent()->InitAbilityActorInfo(TagCharacter, TagCharacter);
	}
}

void ATagPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (ScoreboardAction)
		{
			EnhancedInputComponent->BindAction(ScoreboardAction, ETriggerEvent::Started, this, &ThisClass::ShowScoreboard);
			EnhancedInputComponent->BindAction(ScoreboardAction, ETriggerEvent::Completed, this, &ThisClass::HideScoreboard);
		}
		if (PauseMenuAction)
		{
			EnhancedInputComponent->BindAction(PauseMenuAction, ETriggerEvent::Started, this, &ThisClass::ShowPauseMenu);
		}
	}
}

void ATagPlayerController::ShowPauseMenu()
{
	UKismetSystemLibrary::PrintString(this, "Pausing");

	if (PauseMenuRef)
	{
		HidePauseMenu();
		return;
	}
	if (PauseMenuClass)
	{
		PauseMenuRef = CreateWidget<UPauseMenu>(this, PauseMenuClass);
		if (PauseMenuRef)
		{
			const FInputModeUIOnly InputModeUIOnly;
			SetInputMode(InputModeUIOnly);
			SetShowMouseCursor(true);
			PauseMenuRef->AddToViewport();
		}
	}
}

void ATagPlayerController::HidePauseMenu()
{
	const FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);
	SetShowMouseCursor(false);
	if (PauseMenuRef)
	{
		PauseMenuRef->RemoveFromParent();
		PauseMenuRef = nullptr;
	}
}
