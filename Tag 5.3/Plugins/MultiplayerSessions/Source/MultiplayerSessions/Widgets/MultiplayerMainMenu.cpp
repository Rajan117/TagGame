// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiplayerMainMenu.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Components/Button.h"
#include "HostMenu/HostMenu.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MultiplayerSessions/Subsystems/MultiplayerSessionsSubsystem.h"
#include "ServerBrowser/ServerBrowser.h"

void UMultiplayerMainMenu::MenuSetup()
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	if (const UWorld* World = GetWorld())
	{
		if (APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}

	if (const UGameInstance* GameInstance = GetGameInstance())
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}
}

bool UMultiplayerMainMenu::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (HostButton)
	{
		HostButton->OnClicked.AddDynamic(this, &ThisClass::HostButtonClicked);
	}
	if (JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this, &ThisClass::JoinButtonClicked);
	}

	return true;
}

void UMultiplayerMainMenu::NativeDestruct()
{
	Super::NativeDestruct();
}

void UMultiplayerMainMenu::MenuTearDown()
{
	if (const UWorld* World = GetWorld())
	{
		if (APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			const FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
		}
	}
	RemoveFromParent();
}

void UMultiplayerMainMenu::HostButtonClicked()
{
	if (HostMenuClass)
	{
		if (UHostMenu* HostMenu = CreateWidget<UHostMenu>(this, HostMenuClass))
		{
			if (HostButton)
			{
				HostButton->SetIsEnabled(false);
			}
			HostMenu->AddToViewport();
			RemoveFromParent();
		}
	}
}

void UMultiplayerMainMenu::JoinButtonClicked()
{
	if (BrowserClass)
	{
		if (UServerBrowser* Browser = CreateWidget<UServerBrowser>(this, BrowserClass))
		{
			if (JoinButton)
			{
				JoinButton->SetIsEnabled(false);
			}
			Browser->AddToViewport();
			RemoveFromParent();
		}
	}
}
