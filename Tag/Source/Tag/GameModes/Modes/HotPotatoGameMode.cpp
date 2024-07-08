// Fill out your copyright notice in the Description page of Project Settings.


#include "HotPotatoGameMode.h"

#include "GameFramework/PlayerState.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Tag/Character/TagCharacter.h"
#include "Tag/Controller/TagPlayerController.h"
#include "Tag/GameStates/TagRoundBasedGameState.h"
#include "Tag/PlayerState/TagPlayerState.h"


AHotPotatoGameMode::AHotPotatoGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	NumRounds = 0;
}

void AHotPotatoGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHotPotatoGameMode::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHotPotatoGameMode::StartGame()
{
	Super::StartGame();
}

void AHotPotatoGameMode::StartRound()
{
	Super::StartRound();
	ChooseTagger();
}

void AHotPotatoGameMode::EndRound()
{
	Super::EndRound();
	EliminateTaggedPlayers();
}

void AHotPotatoGameMode::EliminateTaggedPlayers()
{
	for(FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		if (ATagPlayerController* TagPlayerController = Cast<ATagPlayerController>(Iterator->Get()))
		{
			if (const ATagCharacter* TagCharacter = Cast<ATagCharacter>(TagPlayerController->GetCharacter()))
			{
				if (TagCharacter->GetIsTagged())
				{
					RemoveTaggedEffect(TagCharacter);
					EliminatePlayer(TagPlayerController);
				}
			}
		}
	}
	if (GetNumPlayers()-EliminatedPlayers.Num() <= 1) SetMatchState(MatchState::PostMatch);
}

void AHotPotatoGameMode::EliminatePlayer(ATagPlayerController* TagPlayerController)
{
	AnnounceElimination(TagPlayerController->GetPlayerState<ATagPlayerState>());
	EliminatedPlayers.Add(TagPlayerController);
	SwitchPlayerToSpectator(TagPlayerController);
}

void AHotPotatoGameMode::AnnounceElimination(ATagPlayerState* EliminatedPLayer) const
{
	if (TagRoundBasedGameState) TagRoundBasedGameState->Multicast_BroadcastPlayerEliminated(EliminatedPLayer);
}





