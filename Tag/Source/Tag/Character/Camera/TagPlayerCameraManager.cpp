// Fill out your copyright notice in the Description page of Project Settings.


#include "TagPlayerCameraManager.h"

#include "Tag/Character/TagCharacter.h"


// Sets default values
ATagPlayerCameraManager::ATagPlayerCameraManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATagPlayerCameraManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATagPlayerCameraManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATagPlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);

	if (ATagCharacter* TagCharacter = Cast<ATagCharacter>(GetOwningPlayerController()->GetPawn()))
	{
		
	}
}

