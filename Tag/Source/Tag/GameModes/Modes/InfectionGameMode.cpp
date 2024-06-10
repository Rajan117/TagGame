// Fill out your copyright notice in the Description page of Project Settings.


#include "InfectionGameMode.h"

#include "Tag/Character/TagCharacter.h"
#include "Tag/Controller/TagPlayerController.h"


AInfectionGameMode::AInfectionGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AInfectionGameMode::HandleTagEvent(ATagCharacter* TaggingCharacter, ATagCharacter* TaggedCharacter,
	ATagPlayerState* TaggingPlayer, ATagPlayerState* TaggedPlayer)
{
	if (TryTag(TaggedCharacter))
	{
		AnnounceTag(TaggingPlayer, TaggedPlayer);
	}
	if (GetNumUntaggedPlayers() <= 0)
	{
		SetMatchState(MatchState::PostMatch);
	}
}

int32 AInfectionGameMode::GetNumUntaggedPlayers()
{
	int32 Count = 0;
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (const ATagPlayerController* TagPlayerController = Cast<ATagPlayerController>(*It))
		{
			if (const ATagCharacter* TagCharacter = Cast<ATagCharacter>(TagPlayerController->GetCharacter()))
			{
				if (!TagCharacter->GetIsTagged()) Count++;
			}
		}
	}
	return Count;
}

