// Fill out your copyright notice in the Description page of Project Settings.


#include "RoundCountdownTimer.h"

#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Tag/GameModes/TagGameMode.h"
#include "Tag/GameStates/TagGameState.h"

void URoundCountdownTimer::NativeConstruct()
{
	Super::NativeConstruct();
	TagGameState = Cast<ATagGameState>(GetWorld()->GetGameState());
	if (TagGameState)
	{
		TagGameState->OnRoundStartedDelegate.AddDynamic(this, &URoundCountdownTimer::OnRoundStarted);
		TagGameState->OnRoundEndedDelegate.AddDynamic(this, &URoundCountdownTimer::OnRoundEnded);
		SetTimerText(TagGameState->GetCurrentRoundTime());
	}
}

void URoundCountdownTimer::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	// if (TagGameState && bRoundActive)
	// {
	// 	const float ElapsedTime = TagGameState->GetServerWorldTimeSeconds()-StartTime;
	// 	float TimeLeft = TimePeriod - ElapsedTime;
	// 	if (TimeLeft<0.f) TimeLeft = 0.f;
	// 	SetTimerText(TimeLeft);
	// }
	if (!TagGameState) return;
	
	const float ServerTime = TagGameState->GetServerWorldTimeSeconds();
	float SecondsLeft = TagGameState->CurrentRoundTime;
	
	if (TagGameState->GetMatchState() == MatchState::RoundStart)
	{
		const float Elapsed = ServerTime - TagGameState->PhaseStartTime;
		const float Remaining = TagGameState->CurrentRoundTime - Elapsed;
		SecondsLeft = FMath::CeilToInt(FMath::Max(Remaining, 0.f));
	}
	SetTimerText(SecondsLeft);
	
}

void URoundCountdownTimer::OnRoundStarted(float RoundTime)
{
	TimePeriod = RoundTime;
	if (TagGameState) StartTime = TagGameState->GetServerWorldTimeSeconds();
	bRoundActive = true;
}

void URoundCountdownTimer::OnRoundEnded(float RoundIntervalTime)
{
	TimePeriod = RoundIntervalTime;
	if (TagGameState) StartTime = TagGameState->GetServerWorldTimeSeconds();
	bRoundActive = false;
}


void URoundCountdownTimer::SetTimerText(const float Time) const
{
	const int32 Minutes = FMath::FloorToInt(Time/60);
	const int32 Seconds = Time - (Minutes*60);
	const FString TimeText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
	CountdownText->SetText(FText::FromString(TimeText));
}

