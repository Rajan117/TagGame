// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStartTimer.h"

#include "Components/TextBlock.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Tag/Controller/TagPlayerController.h"
#include "Tag/GameModes/TagGameMode.h"
#include "Tag/GameStates/TagGameState.h"

void UGameStartTimer::NativeConstruct()
{
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::Hidden);
	TagPlayerController = Cast<ATagPlayerController>(GetOwningPlayer());
	if (TagPlayerController)
	{
		if (TagPlayerController->GetCharacter()) SetupDelegate(nullptr, TagPlayerController->GetCharacter());
		else TagPlayerController->OnPossessedPawnChanged.AddDynamic(this, &UGameStartTimer::SetupDelegate);
	}
}

void UGameStartTimer::StartTimer(const float Time)
{
	SetVisibility(ESlateVisibility::Visible);
	WarmupTime = FMath::CeilToInt(Time);
	//CountdownText->SetText(FText::FromString(FString::FromInt(WarmupTime)));
	GetWorld()->GetTimerManager().SetTimer(
	  CountdownTimerHandle,
	  this,
	  &UGameStartTimer::CountdownTick,
	  1.f,
	  true
	);
}

void UGameStartTimer::SetTime(float Time)
{
	WarmupTime = FMath::CeilToInt(Time);
}

void UGameStartTimer::CountdownTick()
{
	WarmupTime--;
	CountdownText->SetText(FText::FromString(FString::FromInt(WarmupTime)));
	if (WarmupTime<=0)
	{
		GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
		RemoveFromParent();
	}
}

void UGameStartTimer::OnMatchStateChanged(FName NewState)
{
	if (!TagGameState) return;
	if (NewState == MatchState::Warmup)
	{
		StartTimer(TagGameState->WarmupTime-
			TagGameState->GetServerWorldTimeSeconds()+
			TagGameState->LevelStartingTime);
	}
	else 
	{
		GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
		RemoveFromParent();
	}
}

void UGameStartTimer::SetupDelegate(APawn* OldPawn, APawn* NewPawn)
{
	if (TagPlayerController)
	{
		TagGameState = TagPlayerController->GetTagGameState();
		if (TagGameState && TagGameState->GetMatchState() == MatchState::Warmup)
		{
			StartTimer(TagGameState->WarmupTime-
				TagGameState->GetServerWorldTimeSeconds()+
				TagGameState->LevelStartingTime);
		}
		else if (TagGameState)
		{
			TagGameState->OnMatchStateChangedDelegate.AddDynamic(this, &UGameStartTimer::OnMatchStateChanged);
		}
	}
}
