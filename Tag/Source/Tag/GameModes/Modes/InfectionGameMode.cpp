// Fill out your copyright notice in the Description page of Project Settings.


#include "InfectionGameMode.h"


// Sets default values
AInfectionGameMode::AInfectionGameMode()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AInfectionGameMode::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInfectionGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

