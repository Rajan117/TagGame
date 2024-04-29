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
		PlayerState->ScoreUpdateDelegate.BindUObject(this, &UScoreboardPlayerRow::ScoreUpdated);

		PlayerNameText->SetText(FText::FromString(PlayerState->GetPlayerName()));
		ScoreText->SetText(FText::FromString(FString::SanitizeFloat(PlayerState->GetScore())));
	}
}

void UScoreboardPlayerRow::ScoreUpdated(float NewScore) const
{
	ScoreText->SetText(FText::FromString(FString::SanitizeFloat(NewScore)));
	
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
