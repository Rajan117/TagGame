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

void AHotPotatoGameMode::EndRound()
{
	Super::EndRound();
	EliminateTaggedPlayers();
}







