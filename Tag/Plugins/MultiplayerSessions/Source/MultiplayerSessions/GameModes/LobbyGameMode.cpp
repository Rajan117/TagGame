// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "MultiplayerSessions/Controllers/LobbyPlayerController.h"

ALobbyGameMode::ALobbyGameMode()
{
	
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (ALobbyPlayerController* LobbyController = Cast<ALobbyPlayerController>(NewPlayer))
	{
		LobbyController->ClientCreateLobbyScreen();
	}

	FTimerHandle UpdateHandle;
	GetWorldTimerManager().SetTimer(UpdateHandle, this, &ALobbyGameMode::UpdatePlayerList, 1.0f, false);
}

void ALobbyGameMode::UpdatePlayerList()
{
	if (GetWorld())
	{
		TArray<AActor*> Controllers;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALobbyPlayerController::StaticClass(), Controllers);
		for (int i = 0; i < Controllers.Num(); i++)
		{
			if (ALobbyPlayerController* Controller = Cast<ALobbyPlayerController>(Controllers[i]))
			{
				Controller->ClientUpdatePlayerList();
			}
		}
	}
}
