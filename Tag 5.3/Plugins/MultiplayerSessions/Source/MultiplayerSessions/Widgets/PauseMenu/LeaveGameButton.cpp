// Fill out your copyright notice in the Description page of Project Settings.


#include "LeaveGameButton.h"

#include "Components/Button.h"
#include "MultiplayerSessions/Subsystems/MultiplayerSessionsSubsystem.h"

void ULeaveGameButton::NativeConstruct()
{
	Super::NativeConstruct();
	if (const UGameInstance* GameInstance = GetGameInstance())
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
		if (MultiplayerSessionsSubsystem)
		{
			MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &ULeaveGameButton::OnDestroySession);
		}
	}
	if (LeaveButton)
	{
		LeaveButton->OnClicked.AddDynamic(this, &ULeaveGameButton::OnLeaveButtonClicked);
	}
}

void ULeaveGameButton::ShowLoadingWidget()
{
	if (LoadingWidgetClass)
	{
		if (UUserWidget* LoadingWidgetRef = CreateWidget<UUserWidget>(this, LoadingWidgetClass))
		{
			LoadingWidgetRef->AddToViewport();
		}
	}
}

void ULeaveGameButton::OnLeaveButtonClicked()
{
	if (MultiplayerSessionsSubsystem)
	{
		if (LeaveButton) LeaveButton->SetIsEnabled(false);
		
		MultiplayerSessionsSubsystem->DestroySession();
	}
}

void ULeaveGameButton::OnDestroySession(bool bWasSuccessful)
{
	if (GetWorld())
	{
		ShowLoadingWidget();
		RemoveFromParent();
		
		if (APlayerController* PlayerController = GetOwningPlayer())
		{
			if (PlayerController->HasAuthority())
			{
				GetWorld()->ServerTravel(MainMenuMapAddress, true);
			}
			else
			{
				PlayerController->ClientTravel(MainMenuMapAddress, ETravelType::TRAVEL_Absolute);
			}
		}
	}
}
