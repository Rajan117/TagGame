// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyMenu.h"

#include "LobbyPlayerRow.h"
#include "MapSelector.h"
#include "ModeSelector.h"
#include "Components/Button.h"
#include "OnlineSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MultiplayerSessions/GameStates/LobbyGameState.h"
#include "MultiplayerSessions/Subsystems/MultiplayerSessionsSubsystem.h"

void ULobbyMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		OnDestroySessionCompleteDelegate.BindUObject(this, &ULobbyMenu::OnDestroySessionComplete);
		if (SessionInterface = OnlineSubsystem->GetSessionInterface(); SessionInterface.IsValid())
		{
			OnDestroySessionCompleteDelegateHandle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);
		}
	}
	
	if (const UGameInstance* GameInstance = GetGameInstance())
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
		if (MultiplayerSessionsSubsystem)
		{
			MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &ULobbyMenu::OnDestroySession);
		}
	}
	
	// Bind to game starting event so clients can show loading screen immediately
	if (GetWorld())
	{
		if (ALobbyGameState* LobbyGameState = Cast<ALobbyGameState>(GetWorld()->GetGameState()))
		{
			LobbyGameState->OnGameStartingDelegate.AddDynamic(this, &ULobbyMenu::OnGameStarting);
		}
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
			StartButton->OnClicked.AddDynamic(this, &ULobbyMenu::StartButtonClicked);
		}
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
		
		if (ALobbyGameState* LobbyGameState = Cast<ALobbyGameState>(UGameplayStatics::GetGameState(GetWorld())))
		{
			LobbyGameState->Multicast_NotifyGameStarting();
		}

		FTimerHandle StartTimer;
		GetWorld()->GetTimerManager().SetTimer(StartTimer, this, &ULobbyMenu::LoadMap, 0.5f, false);
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

void ULobbyMenu::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);
	ShowLoadingWidget();
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

void ULobbyMenu::OnGameStarting()
{
	ShowLoadingWidget();
	RemoveFromParent();
}
