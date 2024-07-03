// Fill out your copyright notice in the Description page of Project Settings.


#include "TagHUD.h"

#include "CharacterOverlay.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Tag/Controller/TagPlayerController.h"

void ATagHUD::DrawHUD()
{
	Super::DrawHUD();
}

void ATagHUD::BeginPlay()
{
	Super::BeginPlay();
	TagPlayerController = Cast<ATagPlayerController>(GetOwningPlayerController());
	if (TagPlayerController)
	{
		if (TagPlayerController->GetCharacter()) SetupDelegate(nullptr, TagPlayerController->GetCharacter());
		else TagPlayerController->OnPossessedPawnChanged.AddDynamic(this, &ATagHUD::SetupDelegate);
	}
}

void ATagHUD::AddCharacterOverlay()
{
	TagPlayerController = Cast<ATagPlayerController>(GetOwningPlayerController());
	if (TagPlayerController && CharacterOverlayClass)
	{
		CharacterOverlay = CreateWidget<UCharacterOverlay>(TagPlayerController, CharacterOverlayClass);
		if (CharacterOverlay)
		{
			CharacterOverlay->AddToViewport();
		}
	}
}

void ATagHUD::RemoveCharacterOverlay() const
{
	if (CharacterOverlay)
	{
		CharacterOverlay->RemoveFromParent();
	}
}

void ATagHUD::SetupDelegate(APawn* OldPawn, APawn* NewPawn)
{
	AddCharacterOverlay();
}

