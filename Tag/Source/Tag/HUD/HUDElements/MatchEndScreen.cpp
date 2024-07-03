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
	SetVisibility(ESlateVisibility::Hidden);
	TagPlayerController = Cast<ATagPlayerController>(GetOwningPlayer());
	if (TagPlayerController)
	{
		if (TagPlayerController->GetCharacter()) SetupDelegate(nullptr, TagPlayerController->GetCharacter());
		else TagPlayerController->OnPossessedPawnChanged.AddDynamic(this, &UMatchEndScreen::SetupDelegate);
	}
}

void UMatchEndScreen::OnMatchStateChanged(FName NewState)
{
	if (NewState == MatchState::PostMatch)
	{
		HandlePostMatch();
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMatchEndScreen::SetupDelegate(APawn* OldPawn, APawn* NewPawn)
{
	if (TagPlayerController)
	{
		TagGameState = TagPlayerController->GetTagGameState();
		if (TagGameState)
		{
			TagGameState->OnMatchStateChangedDelegate.AddDynamic(this, &UMatchEndScreen::OnMatchStateChanged);
			if (TagGameState->GetMatchState() == MatchState::PostMatch) HandlePostMatch();
		}
	}
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

void UMatchEndScreen::HandlePostMatch()
{
	SetVisibility(ESlateVisibility::Visible);
	if (TagGameState) StartTimer(TagGameState->RestartTime);
	if (TagPlayerController)
	{
		TagPlayerController->ShowScoreboard();
		if (ATagCharacter* TagCharacter = Cast<ATagCharacter>(TagPlayerController ->GetCharacter()))
		{
			TagCharacter->bShouldUpdateScore = false;
			TagCharacter->DisableInput(TagPlayerController);
		}
	}
}
