// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStartTimer.h"

#include "Components/TextBlock.h"

void UGameStartTimer::NativeConstruct()
{
	Super::NativeConstruct();

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
	Count--;
	CountdownText->SetText(FText::FromString(FString::FromInt(Count)));
	if (Count<=0)
	{
		GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
		RemoveFromParent();
	}
}
