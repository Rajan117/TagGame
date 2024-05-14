// Fill out your copyright notice in the Description page of Project Settings.


#include "Scoreboard.h"

#include "ScoreboardPlayerRow.h"
#include "Components/VerticalBox.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Tag/PlayerState/TagPlayerState.h"

void UScoreboard::NativeConstruct()
{
	Super::NativeConstruct();
	Setup();
}

void UScoreboard::AddPlayer(ATagPlayerState* State)
{
	if (!RowClass) return;
	UScoreboardPlayerRow* NewRow = CreateWidget<UScoreboardPlayerRow>(this, RowClass);
	Rows.Add(NewRow);
	PlayerBox->AddChild(NewRow);
	NewRow->SpawnInitialize(State, this);
}

void UScoreboard::Setup()
{
	if (GetWorld())
	{
		if (AGameStateBase* GameState = UGameplayStatics::GetGameState(GetWorld()))
		{
			TArray<APlayerState*> PlayerStates = GameState->PlayerArray;
			for (int i = 0; i < PlayerStates.Num(); i++)
			{
				if (ATagPlayerState* TagPlayerState = Cast<ATagPlayerState>(PlayerStates[i]))
				{
					AddPlayer(TagPlayerState);
					SortPlayers();
				}
			}
		}
	}
}

void UScoreboard::SortPlayers()
{
	Rows.StableSort([](const UScoreboardPlayerRow& Row1, const UScoreboardPlayerRow& Row2)
	{
			return Row1.GetScore() < Row2.GetScore();
	});

	if (PlayerBox)
	{
		for (int i = 0; i < Rows.Num(); i++)
		{
			if (UScoreboardPlayerRow* Row = Rows[i])
			{
				PlayerBox->AddChildToVerticalBox(Row);
			}
		}
	}
}
