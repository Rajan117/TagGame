// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "MultiplayerSessions/Widgets/Lobby/LobbyMenu.h"

ALobbyPlayerController::ALobbyPlayerController()
{

}

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	const FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);
	SetShowMouseCursor(true);
}

void ALobbyPlayerController::ClientCreateLobbyScreen_Implementation()
{
	if (LobbyClass)
	{
		LobbyMenu = CreateWidget<ULobbyMenu>(this, LobbyClass);
		if (LobbyMenu)
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			SetInputMode(InputModeData);
			SetShowMouseCursor(true);

			LobbyMenu->AddToViewport();
		}
	}
}

void ALobbyPlayerController::ClientUpdatePlayerList_Implementation()
{
	if (LobbyMenu)
	{
		LobbyMenu->Refresh();
	}
}

void ALobbyPlayerController::ClientRemoveMenu_Implementation()
{
	if (LobbyMenu)
	{
		LobbyMenu->RemoveFromParent();
	}
}


