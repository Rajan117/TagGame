// Fill out your copyright notice in the Description page of Project Settings.


#include "HotPotatoGameMode.h"

#include "Tag/Character/TagCharacter.h"
#include "Tag/Controller/TagPlayerController.h"


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
	for(FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		if (ATagPlayerController* TagPlayerController = Cast<ATagPlayerController>(Iterator->Get()))
		{
			if (const ATagCharacter* TagCharacter = Cast<ATagCharacter>(TagPlayerController->GetCharacter()))
			{
				if (TagCharacter->GetIsTagged())
				{
					EliminatePlayer(TagPlayerController);
				}
			}
		}
	}
	
}

void AHotPotatoGameMode::EliminatePlayer(ATagPlayerController* TagPlayerController)
{
	SurvivingPlayers.Remove(TagPlayerController);
	EliminatedPlayers.Add(TagPlayerController);
}





