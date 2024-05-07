// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyMenu.h"

#include "LobbyPlayerRow.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MultiplayerSessions/GameStates/LobbyGameState.h"
#include "MultiplayerSessions/Subsystems/MultiplayerSessionsSubsystem.h"

void ULobbyMenu::Refresh()
{
	if (PlayerBox)
	{
		PlayerBox->ClearChildren();
		if (GetWorld())
		{
			if (AGameStateBase* GameState = UGameplayStatics::GetGameState(GetWorld()))
			{
				TArray<APlayerState*> States = GameState->PlayerArray;
				for (int i = 0; i < States.Num(); i++)
				{
					APlayerState* State = States[i];
					if (PlayerRowClass)
					{
						ULobbyPlayerRow* Row = CreateWidget<ULobbyPlayerRow>(this, PlayerRowClass);
						Row->SpawnInitialize(State);
						if (Row)
						{
							PlayerBox->AddChild(Row);
						}
					}
				}
			}
		}
	}
}

void ULobbyMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (const UGameInstance* GameInstance = GetGameInstance())
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}

	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &ULobbyMenu::OnDestroySession);
	}

	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &ULobbyMenu::BackButtonClicked);
	}
	
	if (StartButton)
	{
		if (GetWorld())
		{
			StartButton->SetIsEnabled(UKismetSystemLibrary::IsServer(GetWorld()));
		}
		StartButton->OnClicked.AddDynamic(this, &ULobbyMenu::StartButtonClicked);
	}

	if (RefreshButton)
	{
		RefreshButton->OnClicked.AddDynamic(this, &ULobbyMenu::Refresh);
	}

	Refresh();
}

void ULobbyMenu::BackButtonClicked()
{
	if (MultiplayerSessionsSubsystem)
	{
		if (BackButton) BackButton->SetIsEnabled(false);
		MultiplayerSessionsSubsystem->DestroySession();
	}
}

void ULobbyMenu::StartButtonClicked()
{
	if (GetWorld())
	{
		if (StartButton)
		{
			StartButton->SetIsEnabled(false);
		}
		
		if (GetWorld())
		{
			if (ALobbyGameState* LobbyGameState = Cast<ALobbyGameState>(UGameplayStatics::GetGameState(GetWorld())))
			{
				//LobbyGameState->ServerRemoveMenu();
			}
		}

		if (GetWorld())
		{
			FTimerHandle StartTimer;
			GetWorld()->GetTimerManager().SetTimer(StartTimer, this, &ULobbyMenu::LoadMap, 1.0f, false);
		}
	}
}

void ULobbyMenu::OnDestroySession(bool bWasSuccessful)
{
	if (GetWorld())
	{
		RemoveFromParent();
		GetWorld()->ServerTravel(StartMapAddress);
	}
}

void ULobbyMenu::LoadMap()
{
	UWorld* World = GetWorld();
	if (World)
	{
		World->ServerTravel("/Game/Tag/Maps/TestMap?listen");
	}
}
