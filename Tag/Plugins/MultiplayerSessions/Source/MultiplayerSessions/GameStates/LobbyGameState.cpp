// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameState.h"

ALobbyGameState::ALobbyGameState()
{

}

void ALobbyGameState::Multicast_BroadcastSessionSettingsChanged_Implementation()
{
	OnSessionSettingsChangedDelegate.Broadcast();
}

