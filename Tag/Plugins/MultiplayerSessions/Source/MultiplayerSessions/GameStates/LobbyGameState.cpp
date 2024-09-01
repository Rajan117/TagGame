// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameState.h"

#include "OnlineSessionSettings.h"
#include "Kismet/GameplayStatics.h"
#include "MultiplayerSessions/Controllers/LobbyPlayerController.h"


ALobbyGameState::ALobbyGameState()
{

}

void ALobbyGameState::Multicast_BroadcastSessionSettingsChanged_Implementation()
{
	OnSessionSettingsChangedDelegate.Broadcast();
}

