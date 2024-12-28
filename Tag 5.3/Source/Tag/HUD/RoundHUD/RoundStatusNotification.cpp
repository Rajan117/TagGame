// Fill out your copyright notice in the Description page of Project Settings.


#include "RoundStatusNotification.h"

#include "Components/TextBlock.h"
#include "Tag/GameStates/TagGameState.h"

void URoundStatusNotification::NativeConstruct()
{
	Super::NativeConstruct();
	TagGameState = Cast<ATagGameState>(GetWorld()->GetGameState());
	if (TagGameState)
	{
		TagGameState->OnRoundStartedDelegate.AddDynamic(this, &URoundStatusNotification::OnRoundStarted);
		TagGameState->OnRoundEndedDelegate.AddDynamic(this, &URoundStatusNotification::OnRoundEnded);
	}
}

void URoundStatusNotification::OnRoundStarted(const float RoundTime)
{
	StatusText->SetText(FText::FromString("Round Ends In: "));
	GetWorld()->GetTimerManager().SetTimer(
		ResetStatusTextTimer,
		this,
		&URoundStatusNotification::ResetStatusText,
		RoundTime,
		false);
}

void URoundStatusNotification::OnRoundEnded(const float IntervalTime)
{
	StatusText->SetText(FText::FromString("New Round In: "));
	GetWorld()->GetTimerManager().SetTimer(
	ResetStatusTextTimer,
	this,
	&URoundStatusNotification::ResetStatusText,
	IntervalTime,
	false);
}

void URoundStatusNotification::ResetStatusText() const
{
	StatusText->SetText(FText::FromString(""));
}

