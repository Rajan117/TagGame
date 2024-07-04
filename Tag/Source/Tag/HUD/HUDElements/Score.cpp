// Fill out your copyright notice in the Description page of Project Settings.


#include "Score.h"

#include "Components/TextBlock.h"
#include "Tag/PlayerState/TagPlayerState.h"

void UScore::NativeConstruct()
{
	Super::NativeConstruct();
	TagPlayerState = Cast<ATagPlayerState>(GetOwningPlayerState());
	if (TagPlayerState)
	{
		TagPlayerState->ScoreUpdatedDelegate.AddDynamic(this, &UScore::OnScoreUpdated);
	}
}

void UScore::OnScoreUpdated(const float Score)
{
	const int32 Minutes = FMath::FloorToInt(Score/60);
	const int32 Seconds = Score - (Minutes*60);
	const FString TimerText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
	ScoreText->SetText(FText::FromString(TimerText));
}
