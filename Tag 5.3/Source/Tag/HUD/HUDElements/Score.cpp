// Fill out your copyright notice in the Description page of Project Settings.


#include "Score.h"

#include "GameTimer.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"

#include "Tag/Controller/TagPlayerController.h"
#include "Tag/PlayerState/TagPlayerState.h"

void UScore::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (!TagPlayerState)
	{
		TagPlayerState = GetOwningPlayer()->GetPlayerState<ATagPlayerState>();
		if (TagPlayerState) TagPlayerState->ScoreUpdatedDelegate.AddDynamic(this, &UScore::OnScoreUpdated);
	}
}

void UScore::OnScoreUpdated(const float Score)
{
	
	const int32 Minutes = FMath::FloorToInt(Score/60);
	const int32 Seconds = Score - (Minutes*60);
	const FString TimerText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
	ScoreText->SetText(FText::FromString(TimerText));
}
