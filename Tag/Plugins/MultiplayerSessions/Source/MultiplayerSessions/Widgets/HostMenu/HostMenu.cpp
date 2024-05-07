// Fill out your copyright notice in the Description page of Project Settings.


#include "HostMenu.h"

#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MultiplayerSessions/Subsystems/MultiplayerSessionsSubsystem.h"
#include "MultiplayerSessions/Widgets/MultiplayerMainMenu.h"

void UHostMenu::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UHostMenu::BackButtonClicked);
	}

	if (HostButton)
	{
		HostButton->OnClicked.AddDynamic(this, &UHostMenu::HostButtonClicked);
	}

	if (PlayerSlider)
	{
		PlayerSlider->OnValueChanged.AddDynamic(this, &UHostMenu::PlayerSliderChanged);
	}

	if (const UGameInstance* GameInstance = GetGameInstance())
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}

	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSession);
	}
}

void UHostMenu::BackButtonClicked()
{
	if (MenuClass)
	{
		if (UMultiplayerMainMenu* Menu = CreateWidget<UMultiplayerMainMenu>(this, MenuClass))
		{
			if (BackButton) BackButton->SetIsEnabled(false);
			Menu->AddToViewport();
			Menu->MenuSetup();
			RemoveFromParent();
		}
	}
}

void UHostMenu::HostButtonClicked()
{
	if (HostButton)
	{
		HostButton->SetIsEnabled(false);
	}
	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->CreateSession(MaxPlayerCount, FString("Lobby"));
	}
}

void UHostMenu::PlayerSliderChanged(float Value)
{
	MaxPlayerCount = FMath::TruncToInt(FMath::Clamp(Value, 0.0f, 16.0f));
	CountText->SetText(FText::FromString(FString::FromInt(MaxPlayerCount)));
}

void UHostMenu::OnCreateSession(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UKismetSystemLibrary::PrintString(this, "Travelling to lobby...");

		if (UWorld* World = GetWorld())
		{
			World->ServerTravel("/Game/Tag/Maps/Lobby?listen");
		}
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, "Failed To Create Session");
		HostButton->SetIsEnabled(true);
	}
}
