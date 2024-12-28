// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "MultiplayerSessions/Widgets/Lobby/LobbyMenu.h"
#include "MultiplayerSessions/Widgets/Lobby/PlayerListBox.h"

ALobbyPlayerController::ALobbyPlayerController()
{

}

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	FInputModeUIOnly InputModeUIOnly;
	InputModeUIOnly.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
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
	if (LobbyMenu && LobbyMenu->PlayerListBox)
	{
		LobbyMenu->PlayerListBox->Refresh();
	}
}

void ALobbyPlayerController::ClientRemoveMenu_Implementation()
{
	if (LobbyMenu)
	{
		LobbyMenu->RemoveFromParent();
	}
}


