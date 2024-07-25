// Fill out your copyright notice in the Description page of Project Settings.


#include "LeaveGameButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MultiplayerSessions/Subsystems/MultiplayerSessionsSubsystem.h"

void ULeaveGameButton::NativeConstruct()
{
	Super::NativeConstruct();
	if (const UGameInstance* GameInstance = GetGameInstance())
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}
	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &ULeaveGameButton::OnDestroySession);
	}
	if (LeaveButton)
	{
		LeaveButton->OnClicked.AddDynamic(this, &ULeaveGameButton::OnLeaveButtonClicked);
	}

	if (GetOwningPlayer()->HasAuthority())
	{
		LeaveText->SetText(FText::FromString("Return To Lobby"));
	}
	else
	{
		LeaveText->SetText(FText::FromString("Quit Session"));
	}
}

void ULeaveGameButton::OnLeaveButtonClicked()
{
	if (GetOwningPlayer()->HasAuthority())
	{
		GetWorld()->ServerTravel(MainMenuURL);
	}
	else if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->DestroySession();
	}
}

void ULeaveGameButton::OnDestroySession(bool bWasSuccessful)
{
	UGameplayStatics::OpenLevel(this, FName("MainMenuMap"));
}

