// Fill out your copyright notice in the Description page of Project Settings.


#include "TagHUD.h"

#include "CharacterOverlay.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

void ATagHUD::DrawHUD()
{
	Super::DrawHUD();

	
}

void ATagHUD::BeginPlay()
{
	Super::BeginPlay();
}

void ATagHUD::AddCharacterOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && CharacterOverlayClass)
	{
		CharacterOverlay = CreateWidget<UCharacterOverlay>(PlayerController, CharacterOverlayClass);
		if (CharacterOverlay)
		{
			CharacterOverlay->AddToViewport();
		}
	}
}

