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
	const float TimeLeft = TagGameState->WarmupTime-TagGameState->GetServerWorldTimeSeconds();
	if (TimeLeft <= 0) RemoveFromParent();
	CountdownText->SetText(FText::FromString(FString::FromInt(FMath::FloorToInt(TimeLeft))));
}

void UGameStartTimer::OnMatchStateChanged(FName NewState)
{
	if (!TagGameState) return;
	if (NewState == MatchState::Warmup)
	{
		SetVisibility(ESlateVisibility::Visible);
	}
	else 
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}

void UGameStartTimer::SetupDelegate(APawn* OldPawn, APawn* NewPawn)
{
	TagGameState = Cast<ATagGameState>(GetWorld()->GetGameState());
	if (TagGameState)
	{
		TagGameState->OnMatchStateChangedDelegate.AddDynamic(this, &UGameStartTimer::OnMatchStateChanged);
		if (TagGameState->GetMatchState() == MatchState::Warmup) SetVisibility(ESlateVisibility::Visible);
	}
}
