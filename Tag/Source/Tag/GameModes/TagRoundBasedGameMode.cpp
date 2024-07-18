// Fill out your copyright notice in the Description page of Project Settings.


#include "TagRoundBasedGameMode.h"

#include "GameFramework/PlayerState.h"
#include "GameFramework/SpectatorPawn.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Tag/Character/TagCharacter.h"
#include "Tag/Controller/TagPlayerController.h"
#include "Tag/GameStates/TagRoundBasedGameState.h"

void ATagRoundBasedGameMode::BeginPlay()
{
	Super::BeginPlay();
	TagRoundBasedGameState = Cast<ATagRoundBasedGameState>(GetWorld()->GetGameState());
}

void ATagRoundBasedGameMode::HandleTick(float DeltaSeconds)
{
	if (MatchState == MatchState::WaitingToStart)
	{
		LoadCountdownTime = LoadTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (LoadCountdownTime <= 0.f && GetNumPlayers()>=2)
		{
			StartMatch();
			SetMatchState(MatchState::Warmup);
		}
	}

}

void ATagRoundBasedGameMode::StartGame()
{
	StartRound();
}

void ATagRoundBasedGameMode::StartRound()
{
	RoundStartingTime = GetWorld()->GetTimeSeconds();
	CurrentRound++;
	SetMatchState(MatchState::RoundStart);
	if (TagRoundBasedGameState) TagRoundBasedGameState->Multicast_BroadcastRoundStart(RoundTime);
}

void ATagRoundBasedGameMode::EndRound()
{
	if (NumRounds > 0 && CurrentRound >= NumRounds)
	{
		SetMatchState(MatchState::PostMatch);
	}
	else
	{
		SetMatchState(MatchState::RoundEnd);
		if (TagRoundBasedGameState) TagRoundBasedGameState->Multicast_BroadcastRoundEnd(RoundIntervalTime);
	}
}

void ATagRoundBasedGameMode::SwitchPlayerToSpectator(ATagPlayerController* TagPlayerController) const
{
	if (!TagPlayerController) return;

	if (ATagCharacter* TagCharacter = Cast<ATagCharacter>(TagPlayerController->GetCharacter()))
	{
		TagPlayerController->UnPossess();
		TagCharacter->Destroy();
	}
	
	if (ASpectatorPawn* SpectatorPawn = GetWorld()->SpawnActor<ASpectatorPawn>(ASpectatorPawn::StaticClass(), TagPlayerController->GetSpawnLocation(), FRotator::ZeroRotator))
	{
		TagPlayerController->Possess(SpectatorPawn);
		if (APlayerState* PlayerState = TagPlayerController->PlayerState)
		{
			PlayerState->SetIsSpectator(true);
		}
		TagPlayerController->SetViewTarget(SpectatorPawn);
		TagPlayerController->ChangeState(NAME_Spectating);
	}
}
