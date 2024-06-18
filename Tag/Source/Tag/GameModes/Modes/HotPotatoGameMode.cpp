// Fill out your copyright notice in the Description page of Project Settings.


#include "HotPotatoGameMode.h"


AHotPotatoGameMode::AHotPotatoGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHotPotatoGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHotPotatoGameMode::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHotPotatoGameMode::EliminateTaggedPlayers()
{
	SurvivingPlayers.Remove(CurrentTaggedPlayer);
	EliminatedPlayers.Add(CurrentTaggedPlayer);
	CurrentTaggedPlayer = nullptr;
	CurrentTaggedPlayer = ChooseTagger();
}





