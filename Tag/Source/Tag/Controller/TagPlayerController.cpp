// Fill out your copyright notice in the Description page of Project Settings.


#include "TagPlayerController.h"

#include "Components/TextBlock.h"
#include "Tag/Character/TagCharacter.h"
#include "Tag/HUD/CharacterOverlay.h"
#include "Tag/HUD/TagHUD.h"

void ATagPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TagHUD = TagHUD == nullptr ? Cast<ATagHUD>(GetHUD()) : TagHUD;
}

void ATagPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);

	ATagCharacter* TagCharacter = Cast<ATagCharacter>(P);
	if (TagCharacter)
	{
		TagCharacter->GetAbilitySystemComponent()->InitAbilityActorInfo(TagCharacter, TagCharacter);
	}
}

void ATagPlayerController::SetCurrentEffectHUD(const FString& EffectText)
{
	TagHUD = TagHUD == nullptr ? Cast<ATagHUD>(GetHUD()) : TagHUD;
	if (TagHUD && TagHUD->CharacterOverlay && TagHUD->CharacterOverlay->EffectText)
	{
		TagHUD->CharacterOverlay->EffectText->SetText(FText::FromString(EffectText));
	}
}
