// Fill out your copyright notice in the Description page of Project Settings.


#include "TagPlayerController.h"

#include "Tag/HUD/CharacterOverlay.h"
#include "Tag/HUD/TagHUD.h"

void ATagPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TagHUD = TagHUD == nullptr ? Cast<ATagHUD>(GetHUD()) : TagHUD;
}

void ATagPlayerController::SetCurrentEffectHUD()
{
	TagHUD = TagHUD == nullptr ? Cast<ATagHUD>(GetHUD()) : TagHUD;
	if (TagHUD && TagHUD->CharacterOverlay && TagHUD->CharacterOverlay->EffectText)
	{
		
	}
}
