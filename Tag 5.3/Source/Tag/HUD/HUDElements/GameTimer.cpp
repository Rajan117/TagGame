// Fill out your copyright notice in the Description page of Project Settings.


#include "GameTimer.h"

#include "Components/TextBlock.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Tag/Controller/TagPlayerController.h"
#include "Tag/GameModes/TagGameMode.h"
#include "Tag/GameStates/TagGameState.h"
#include "Tag/PlayerState/TagPlayerState.h"


void UGameTimer::NativeConstruct()
{
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::Hidden);
	TagPlayerController = Cast<ATagPlayerController>(GetOwningPlayer());
	if (TagPlayerController)
	{
		if (TagPlayerController->GetCharacter()) SetupDelegate(nullptr, TagPlayerController->GetCharacter());
		else TagPlayerController->OnPossessedPawnChanged.AddDynamic(this, &UGameTimer::SetupDelegate);
	}
}

void UGameTimer::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	SetHUDTime();
}

void UGameTimer::OnMatchStateChanged(FName NewState)
{
	if (NewState == MatchState::InMatch)
	{
		SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}

void UGameTimer::SetupDelegate(APawn* OldPawn, APawn* NewPawn)
{

	TagGameState = Cast<ATagGameState>(GetWorld()->GetGameState());
	if (TagGameState && TagGameState->GetMatchState() == MatchState::RoundStart)
	{
		SetVisibility(ESlateVisibility::Visible);
	}
	else if (TagGameState)
	{
		TagGameState->OnMatchStateChangedDelegate.AddDynamic(this, &UGameTimer::OnMatchStateChanged);
	}
}

void UGameTimer::SetHUDTime() const
{
	if (!TagGameState) return;
	
	const float ServerTime = TagGameState->GetServerWorldTimeSeconds();
	float SecondsLeft = TagGameState->CurrentRoundTime;
	
	if (TagGameState->GetMatchState() == MatchState::RoundStart)
	{
		const float Elapsed = ServerTime - TagGameState->PhaseStartTime;
		const float Remaining = TagGameState->CurrentRoundTime - Elapsed;
		SecondsLeft = FMath::CeilToInt(FMath::Max(Remaining, 0.f));
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Time Elapsed: %f"), Elapsed), true, true, FLinearColor::Green, 0.01f);
	}
	if (SecondsLeft <= 10)
	{
		TimerText->SetColorAndOpacity(FSlateColor(FLinearColor(1.f, 0.f, 0.f, 1)));
	}
	else
	{
		TimerText->SetColorAndOpacity(FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1)));
	}
	SetHUDTimerText(SecondsLeft);
}

void UGameTimer::SetHUDTimerText(const float Time) const
{
	const int32 Minutes = FMath::FloorToInt(Time/60);
	const int32 Seconds = Time - (Minutes*60);
	const FString TimeText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
	TimerText->SetText(FText::FromString(TimeText));
}
