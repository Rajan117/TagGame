// Fill out your copyright notice in the Description page of Project Settings.


#include "TagRoundBasedGameMode.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Tag/Controller/TagPlayerController.h"

namespace MatchState
{
	const FName RoundStart = FName("RoundStart"); //During a round
	const FName RoundEnd = FName("RoundEnd"); //Round interval
}

void ATagRoundBasedGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ATagRoundBasedGameMode::HandleTick(float DeltaSeconds)
{
	if (MatchState == MatchState::WaitingToStart)
	{
		LoadCountdownTime = LoadTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (LoadCountdownTime <= 0.f && Players.Num()>=2)
		{
			StartMatch();
			SetMatchState(MatchState::Warmup);
		}
	}
	else if (MatchState == MatchState::InMatch &&
		GetWorld()->GetTimeSeconds() - RoundStartingTime >= RoundTime)
	{
		EndRound();
		StartRound();
	}
}

void ATagRoundBasedGameMode::StartGame()
{
	Super::StartGame();
	StartRound();
}

void ATagRoundBasedGameMode::StartRound()
{
	RoundStartingTime = GetWorld()->GetTimeSeconds();
	CurrentRound++;
	UKismetSystemLibrary::PrintString(this, "Starting Round: " + FString::FromInt(CurrentRound));
	SetMatchState(MatchState::RoundStart);
}

void ATagRoundBasedGameMode::EndRound()
{
	UKismetSystemLibrary::PrintString(this, "Ending Round: " + FString::FromInt(CurrentRound));
	if (NumRounds > 0 && CurrentRound >= NumRounds)
	{
		SetMatchState(MatchState::PostMatch);
	}
	else
	{
		SetMatchState(MatchState::RoundEnd);
	}
}
