// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerEliminatedAnnouncement.h"

#include "Components/TextBlock.h"

void UPlayerEliminatedAnnouncement::NativeConstruct()
{
	Super::NativeConstruct();
	GetWorld()->GetTimerManager().SetTimer(
	  AnnouncementTimerHandle,
	  this,
	  &UPlayerEliminatedAnnouncement::RemoveFromParent,
	  AnnouncementTime,
	  true
	);
}

void UPlayerEliminatedAnnouncement::SetEliminationAnnouncement(FString EliminatedPlayerName)
{
	EliminatedPlayerText->SetText(FText::FromString(EliminatedPlayerName));
}
