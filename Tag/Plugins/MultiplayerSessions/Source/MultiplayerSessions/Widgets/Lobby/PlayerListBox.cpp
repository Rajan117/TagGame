// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerListBox.h"

#include "LobbyMenu.h"
#include "LobbyPlayerRow.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "MultiplayerSessions/Subsystems/MultiplayerSessionsSubsystem.h"

void UPlayerListBox::Refresh()
{
	if (PlayerBox && PlayerRowClass)
	{
		PlayerBox->ClearChildren();
		if (GetWorld())
		{
			if (AGameStateBase* GameState = UGameplayStatics::GetGameState(GetWorld()))
			{
				for (auto PlayerState :  GameState->PlayerArray)
				{
					if (PlayerState)
					{
						ULobbyPlayerRow* Row = CreateWidget<ULobbyPlayerRow>(this, PlayerRowClass);
						Row->SpawnInitialize(PlayerState);
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

void UPlayerListBox::NativeConstruct()
{
	Super::NativeConstruct();

	if (const UGameInstance* GameInstance = GetGameInstance())
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}

	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->MultiplayerOnSessionParticipantsChanged.AddUObject(this, &ThisClass::OnSessionParticipantsChanged);
	}

	if (RefreshButton)
	{
		RefreshButton->OnClicked.AddDynamic(this, &ThisClass::RefreshButtonPressed);
	}

	Refresh();
	GetWorld()->GetTimerManager().SetTimer(
	  RefreshTimerHandle,
	  this,
	  &ThisClass::Refresh,
	  1.f,
	  true
	);
}

void UPlayerListBox::OnSessionParticipantsChanged(FName SessionName, const FUniqueNetId& UniqueId, bool bJoined)
{
	UKismetSystemLibrary::PrintString(this, "Participants Changed");
	Refresh();
}

void UPlayerListBox::RefreshButtonPressed()
{
	Refresh();
}
