// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyMenu.h"

#include "LobbyPlayerRow.h"
#include "MapSelector.h"
#include "ModeSelector.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MultiplayerSessions/GameStates/LobbyGameState.h"
#include "MultiplayerSessions/Subsystems/MultiplayerSessionsSubsystem.h"

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
}

void ULobbyMenu::ShowLoadingWidget()
{
	if (LoadingWidgetClass)
	{
		if (UUserWidget* LoadingWidgetRef = CreateWidget<UUserWidget>(this, LoadingWidgetClass))
		{
			LoadingWidgetRef->AddToViewport();
		}
	}
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
		ShowLoadingWidget();
		RemoveFromParent();
		GetWorld()->ServerTravel(StartMapAddress);
	}
}

void ULobbyMenu::LoadMap()
{
	UWorld* World = GetWorld();
	if (World && MapSelector && ModeSelector)
	{
		const FString MapURL = MapSelector->GetSelectedMapURL();
		const FString GameModeURL = ModeSelector->GetSelectedModeURL();
		const FString TravelURL = FString::Printf(TEXT("%s?game=%s?listen"), *MapURL, *GameModeURL);

		UKismetSystemLibrary::PrintString(this, TravelURL);

		ShowLoadingWidget();
		RemoveFromParent();
		World->ServerTravel(TravelURL);
	}
}
