// Fill out your copyright notice in the Description page of Project Settings.


#include "RoundStatusNotification.h"

#include "Components/TextBlock.h"
#include "Tag/Controller/TagPlayerController.h"

void URoundStatusNotification::NativeConstruct()
{
	Super::NativeConstruct();
	TagPlayerController = TagPlayerController == nullptr ? Cast<ATagPlayerController>(GetOwningPlayer()) : TagPlayerController;
	if (TagPlayerController)
	{
		TagPlayerController->OnRoundStartedDelegate.AddDynamic(this, &URoundStatusNotification::OnRoundStarted);
		TagPlayerController->OnRoundEndedDelegate.AddDynamic(this, &URoundStatusNotification::OnRoundEnded);
	}
}

void URoundStatusNotification::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void URoundStatusNotification::OnRoundStarted(float RoundTime)
{
	StatusText->SetText(FText::FromString("Round Started"));
	GetWorld()->GetTimerManager().SetTimer(
		ResetStatusTextTimer,
		this,
		&URoundStatusNotification::ResetStatusText,
		StatusTextResetTime,
		false);
}

void URoundStatusNotification::OnRoundEnded(float RoundIntervalTime)
{
	StatusText->SetText(FText::FromString("Round Ended"));
	GetWorld()->GetTimerManager().SetTimer(
	ResetStatusTextTimer,
	this,
	&URoundStatusNotification::ResetStatusText,
	StatusTextResetTime,
	false);
}

void URoundStatusNotification::ResetStatusText() const
{
	StatusText->SetText(FText::FromString(""));
}

