// Fill out your copyright notice in the Description page of Project Settings.


#include "HotPotatoGameMode.h"

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







