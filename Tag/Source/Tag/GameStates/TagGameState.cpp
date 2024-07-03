// Fill out your copyright notice in the Description page of Project Settings.


#include "TagGameState.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Tag/Character/TagCharacter.h"
#include "Tag/Controller/TagPlayerController.h"
#include "Tag/GameModes/TagGameMode.h"
#include "Tag/HUD/TagHUD.h"
#include "Tag/HUD/HUDElements/MatchEndScreen.h"

class ATagCharacter;

void ATagGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATagGameState, MatchTime);
	DOREPLIFETIME(ATagGameState, WarmupTime);
	DOREPLIFETIME(ATagGameState, RestartTime);
	DOREPLIFETIME(ATagGameState, LevelStartingTime);
	DOREPLIFETIME(ATagGameState, RoundStartingTime);
}

void ATagGameState::OnRep_MatchState()
{
	Super::OnRep_MatchState();

	OnMatchStateChangedDelegate.Broadcast(MatchState);
}