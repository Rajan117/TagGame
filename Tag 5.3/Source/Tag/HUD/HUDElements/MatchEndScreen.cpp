// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchEndScreen.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameFramework/GameMode.h"
#include "Tag/GameModes/TagGameMode.h"

void UMatchEndScreen::NativeConstruct()
{
	Super::NativeConstruct();

	if (GetOwningPlayer()->HasAuthority())
	{
		RestartButton->OnClicked.AddDynamic(this, &UMatchEndScreen::RestartButtonClicked);
	}
	else
	{
		RestartButton->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMatchEndScreen::StartTimer(float Time)
{
	CountdownTime = Time;
	CountdownText->SetText(FText::FromString(FString::FromInt(CountdownTime)));
	GetWorld()->GetTimerManager().SetTimer(
	  CountdownTimerHandle,
	  this,
	  &UMatchEndScreen::CountdownTick,
	  1,
	  true
	);
}

void UMatchEndScreen::CountdownTick()
{
	CountdownTime--;
	CountdownText->SetText(FText::FromString(FString::FromInt(CountdownTime)));
	if (CountdownTime<=0)
	{
		GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
		if (ATagGameMode* TagGameMode = Cast<ATagGameMode>(GetWorld()->GetAuthGameMode()))
		{
			TagGameMode->RestartGame();
		}
	}
}

void UMatchEndScreen::RestartButtonClicked()
{
	if (ATagGameMode* TagGameMode = Cast<ATagGameMode>(GetWorld()->GetAuthGameMode()))
	{
		StartTimer(TagGameMode->GetRestartGameTime());
	}
}
