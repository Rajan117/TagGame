// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchEndScreen.h"

#include "Components/TextBlock.h"
#include "GameFramework/Character.h"
#include "Tag/Character/TagCharacter.h"
#include "Tag/Controller/TagPlayerController.h"
#include "Tag/GameModes/TagGameMode.h"
#include "Tag/GameStates/TagGameState.h"

void UMatchEndScreen::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMatchEndScreen::StartTimer(float Time)
{
	CountdownTime = Time;
	CountdownText->SetText(FText::FromString(FString::FromInt(CountdownTime)));
	GetWorld()->GetTimerManager().SetTimer(
	  CountdownTimerHandle,
	  this,
	  &UMatchEndScreen::CountdownTick,
	  1,
	  true
	);
}

void UMatchEndScreen::CountdownTick()
{
	CountdownTime--;
	CountdownText->SetText(FText::FromString(FString::FromInt(CountdownTime)));
	if (CountdownTime<=0)
	{
		GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
	}
}