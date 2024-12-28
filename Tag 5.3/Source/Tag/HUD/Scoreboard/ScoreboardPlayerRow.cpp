// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreboardPlayerRow.h"

// ReSharper disable once CppUnusedIncludeDirective
#include "Kismet/KismetSystemLibrary.h"
#include "Components/TextBlock.h"

#include "Tag/PlayerState/TagPlayerState.h"
#include "Tag/HUD/Scoreboard/Scoreboard.h"

void UScoreboardPlayerRow::SpawnInitialize(ATagPlayerState* State, UScoreboard* ScoreboardRef)
{
	if (State) PlayerState = State;
	if (ScoreboardRef) Scoreboard = ScoreboardRef;

	if (PlayerState)
	{
		PlayerState->ScoreUpdatedDelegate.AddDynamic(this, &UScoreboardPlayerRow::ScoreUpdated);

		PlayerNameText->SetText(FText::FromString(PlayerState->GetPlayerName()));

		const float Score = PlayerState->GetScore();
		const int32 Minutes = FMath::FloorToInt(Score/60);
		const int32 Seconds = Score - (Minutes*60);
		const FString TimerText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		ScoreText->SetText(FText::FromString(TimerText));
		
		if (PlayerState == GetOwningPlayer()->GetPlayerState<ATagPlayerState>())
		{
			PlayerNameText->SetColorAndOpacity(FSlateColor(FLinearColor(1, 1.f, 0.f, 1)));
			ScoreText->SetColorAndOpacity(FSlateColor(FLinearColor(1, 1.f, 0.f, 1)));
		}
		
	}
}

void UScoreboardPlayerRow::ScoreUpdated(float NewScore)
{
	const int32 Minutes = FMath::FloorToInt(NewScore/60);
	const int32 Seconds = NewScore - (Minutes*60);
	const FString TimerText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
	ScoreText->SetText(FText::FromString(TimerText));
	
	if (Scoreboard)
	{
		Scoreboard->SortPlayers();
	}
}

float UScoreboardPlayerRow::GetScore() const
{
	if (PlayerState) return PlayerState->GetScore();
	return 0.f;
}
