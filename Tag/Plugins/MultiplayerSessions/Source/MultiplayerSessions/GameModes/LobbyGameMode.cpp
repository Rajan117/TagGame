// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionDelegates.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Kismet/GameplayStatics.h"
#include "MultiplayerSessions/Controllers/LobbyPlayerController.h"
#include "MultiplayerSessions/GameStates/LobbyGameState.h"

ALobbyGameMode::ALobbyGameMode()
{
	
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		if (const IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface(); SessionInterface.IsValid())
		{
			OnUpdateSessionCompleteDelegate = SessionInterface->AddOnUpdateSessionCompleteDelegate_Handle(
				FOnUpdateSessionCompleteDelegate::CreateUObject(this, &ALobbyGameMode::OnUpdateSessionComplete)
			);
		}
	}

	LobbyGameState = GetGameState<ALobbyGameState>();
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

void ALobbyGameMode::OnUpdateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	GetWorld()->GetTimerManager().SetTimer(
		BroadcastSessionSettingsUpdatedTimer,
		this,
		&ALobbyGameMode::CallLobbyStateMulticast,
		1.f,
		false
		);
}

void ALobbyGameMode::CallLobbyStateMulticast()
{
	if (LobbyGameState) LobbyGameState->Multicast_BroadcastSessionSettingsChanged();

}
