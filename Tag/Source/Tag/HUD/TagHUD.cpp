// Fill out your copyright notice in the Description page of Project Settings.


#include "TagHUD.h"

#include "CharacterOverlay.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Tag/Character/TagCharacter.h"
#include "Tag/Controller/TagPlayerController.h"
#include "Tag/GameModes/TagGameMode.h"
#include "Tag/GameStates/TagGameState.h"
#include "Tag/HUD/HUDElements/MatchEndScreen.h"

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
	if (TagPlayerController)
	{
		TagGameState = TagPlayerController->GetTagGameState();
		if (TagGameState)
		{
			TagGameState->OnMatchStateChangedDelegate.AddDynamic(this, &ATagHUD::OnMatchStateChanged);
			if (TagGameState->GetMatchState() == MatchState::PostMatch) HandlePostMatch();
		}
	}
}

void ATagHUD::OnMatchStateChanged(FName NewState)
{
	if (NewState == MatchState::PostMatch)
	{
		HandlePostMatch();
	}
}

void ATagHUD::HandlePostMatch()
{
	if (!TagGameState || !TagPlayerController || !MatchEndWidgetClass) return;

	TagPlayerController->ShowScoreboard();
	if (ATagCharacter* TagCharacter = Cast<ATagCharacter>(TagPlayerController ->GetCharacter()))
	{
		TagCharacter->bShouldUpdateScore = false;
		TagCharacter->DisableInput(TagPlayerController);
	}
	
	if (MatchEndWidgetClass && MatchEndWidgetRef == nullptr)
	{
		MatchEndWidgetRef = CreateWidget<UMatchEndScreen>(TagPlayerController, MatchEndWidgetClass);
		if (MatchEndWidgetRef)
		{
			MatchEndWidgetRef->AddToViewport();
			MatchEndWidgetRef->StartTimer(TagGameState->RestartTime);
			RemoveCharacterOverlay();
		}
	}
}

