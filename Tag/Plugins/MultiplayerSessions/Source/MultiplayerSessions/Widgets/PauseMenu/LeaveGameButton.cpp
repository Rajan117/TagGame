// Fill out your copyright notice in the Description page of Project Settings.


#include "LeaveGameButton.h"

#include "Components/Button.h"
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
}

void ULeaveGameButton::OnLeaveButtonClicked()
{
	UKismetSystemLibrary::PrintString(this, "Leave Button Clicked");
	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->DestroySession();
	}
}

void ULeaveGameButton::OnDestroySession(bool bWasSuccessful)
{
	if (GetWorld())
	{
		GetWorld()->ServerTravel(StartMapAddress);
	}
}
