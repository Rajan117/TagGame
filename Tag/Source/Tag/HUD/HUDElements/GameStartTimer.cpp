// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStartTimer.h"

#include "Components/TextBlock.h"

void UGameStartTimer::StartTimer(const float Time)
{
	WarmupTime = Time;
	CountdownText->SetText(FText::FromString(FString::FromInt(WarmupTime)));
	GetWorld()->GetTimerManager().SetTimer(
	  CountdownTimerHandle,
	  this,
	  &UGameStartTimer::CountdownTick,
	  1,
	  true
	);
}

void UGameStartTimer::CountdownTick()
{
	WarmupTime--;
	CountdownText->SetText(FText::FromString(FString::FromInt(WarmupTime)));
	if (WarmupTime<=0)
	{
		GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
		RemoveFromParent();
	}
}
