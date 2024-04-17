// Fill out your copyright notice in the Description page of Project Settings.


#include "TagPlayerController.h"

#include "Components/TextBlock.h"
#include "Tag/Character/TagCharacter.h"
#include "Tag/HUD/CharacterOverlay.h"
#include "Tag/HUD/TagHUD.h"
#include "Tag/HUD/HUDElements/GameTimer.h"

void ATagPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TagHUD = TagHUD == nullptr ? Cast<ATagHUD>(GetHUD()) : TagHUD;
}

void ATagPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	SetHUDTime();
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

void ATagPlayerController::SetHUDTime()
{
	const uint32 SecondsLeft = FMath::CeilToInt(MatchTime-GetWorld()->GetTimeSeconds());
	if (TimerInt != SecondsLeft)
	{
		SetHUDTimerText(SecondsLeft);
	}
	TimerInt = SecondsLeft;
}

void ATagPlayerController::SetCurrentEffectHUD(const FString& EffectText)
{
	TagHUD = TagHUD == nullptr ? Cast<ATagHUD>(GetHUD()) : TagHUD;
	if (TagHUD && TagHUD->CharacterOverlay && TagHUD->CharacterOverlay->EffectText)
	{
		TagHUD->CharacterOverlay->EffectText->SetText(FText::FromString(EffectText));
	}
}

void ATagPlayerController::SetHUDTimerText(const float Time)
{
	TagHUD = TagHUD == nullptr ? Cast<ATagHUD>(GetHUD()) : TagHUD;
	if (TagHUD && TagHUD->CharacterOverlay && TagHUD->CharacterOverlay->GameTimer && TagHUD->CharacterOverlay->GameTimer->TimerText)
	{
		const int32 Minutes = FMath::FloorToInt(Time/60);
		const int32 Seconds = Time - (Minutes*60);
		const FString TimerText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		TagHUD->CharacterOverlay->GameTimer->TimerText->SetText(FText::FromString(TimerText));
	}
}
