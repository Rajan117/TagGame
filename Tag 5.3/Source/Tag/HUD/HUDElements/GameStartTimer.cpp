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

void UGameStartTimer::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	CalcTime();
}

void UGameStartTimer::CalcTime()
{
	if (!TagGameState) return;
	
	// Don't attempt to calculate until we have a valid phase start time replicated from the server
	if (TagGameState->PhaseStartTime <= 0.f) return;

	const float ServerTime = TagGameState->GetServerWorldTimeSeconds();
	float SecondsLeft = TagGameState->WarmupTime;
	
	if (TagGameState->GetMatchState() == MatchState::Warmup)
	{
		SetVisibility(ESlateVisibility::Visible);
		const float Elapsed = ServerTime - TagGameState->PhaseStartTime;
		const float Remaining = TagGameState->WarmupTime - Elapsed;
		// small epsilon to avoid off-by-one due to rounding/latency differences
		const float AdjustedRemaining = FMath::Max(Remaining - 0.01f, 0.f);
		SecondsLeft = FMath::CeilToInt(AdjustedRemaining);
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
	if (SecondsLeft <= 0) RemoveFromParent();
	CountdownText->SetText(FText::FromString(FString::FromInt(FMath::Max(0, FMath::FloorToInt(SecondsLeft)))));
}

void UGameStartTimer::OnMatchStateChanged(FName NewState)
{
	if (!TagGameState) return;
	if (NewState == MatchState::Warmup)
	{
		// Only show the UI if the server has set a valid PhaseStartTime and it has replicated to this client
		if (TagGameState->PhaseStartTime > 0.f)
		{
			SetVisibility(ESlateVisibility::Visible);
		}
	}
	else 
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}
