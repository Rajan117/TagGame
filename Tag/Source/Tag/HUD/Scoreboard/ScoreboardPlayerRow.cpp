// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreboardPlayerRow.h"

#include "Components/TextBlock.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Tag/PlayerState/TagPlayerState.h"

void UScoreboardPlayerRow::SpawnInitialize(ATagPlayerState* State, UScoreboard* ScoreboardRef)
{
	if (State) PlayerState = State;
	if (ScoreboardRef) Scoreboard = ScoreboardRef;

	if (PlayerState)
	{
		UKismetSystemLibrary::PrintString(this, PlayerState->GetPlayerName());
		PlayerNameText->SetText(FText::FromString(PlayerState->GetPlayerName()));
		//ScoreText->SetText(FText::FromString(FString::SanitizeFloat(PlayerState->GetScore())));
	}
}
