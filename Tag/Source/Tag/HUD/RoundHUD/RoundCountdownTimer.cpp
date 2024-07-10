// Fill out your copyright notice in the Description page of Project Settings.


#include "RoundCountdownTimer.h"

#include "Components/TextBlock.h"
#include "GameFramework/Character.h"
#include "Tag/Controller/TagPlayerController.h"
#include "Tag/GameStates/TagRoundBasedGameState.h"

void URoundCountdownTimer::NativeConstruct()
{
	Super::NativeConstruct();
	TagRoundBasedGameState = Cast<ATagRoundBasedGameState>(GetWorld()->GetGameState());
	if (TagRoundBasedGameState)
	{
		TagRoundBasedGameState->OnRoundStartedDelegate.AddDynamic(this, &URoundCountdownTimer::OnRoundStarted);
		TagRoundBasedGameState->OnRoundEndedDelegate.AddDynamic(this, &URoundCountdownTimer::OnRoundEnded);
	}
}

void URoundCountdownTimer::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (TagRoundBasedGameState)
	{
		const float ElapsedTime = TagRoundBasedGameState->GetServerWorldTimeSeconds()-StartTime;
		float TimeLeft = TimePeriod - ElapsedTime;
		if (TimeLeft<0.f) TimeLeft = 0.f;
		SetTimerText(TimeLeft);
	}
}

void URoundCountdownTimer::OnRoundStarted(float RoundTime)
{
	TimePeriod = RoundTime;
	if (TagRoundBasedGameState) StartTime = TagRoundBasedGameState->GetServerWorldTimeSeconds();
}

void URoundCountdownTimer::OnRoundEnded(float RoundIntervalTime)
{
	TimePeriod = RoundIntervalTime;
	if (TagRoundBasedGameState) StartTime = TagRoundBasedGameState->GetServerWorldTimeSeconds();
}


void URoundCountdownTimer::SetTimerText(const float Time) const
{
	const int32 Minutes = FMath::FloorToInt(Time/60);
	const int32 Seconds = Time - (Minutes*60);
	const FString TimeText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
	CountdownText->SetText(FText::FromString(TimeText));
}

