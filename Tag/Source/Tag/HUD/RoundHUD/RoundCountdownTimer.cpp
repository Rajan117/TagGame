// Fill out your copyright notice in the Description page of Project Settings.


#include "RoundCountdownTimer.h"

#include "Components/TextBlock.h"
#include "GameFramework/Character.h"
#include "Tag/Controller/TagPlayerController.h"

void URoundCountdownTimer::NativeConstruct()
{
	Super::NativeConstruct();
	CountdownText->SetText(FText::FromString("00"));
	if (GetOwningPlayer())
	{
		TagPlayerController = TagPlayerController == nullptr ? Cast<ATagPlayerController>(GetOwningPlayer()) : TagPlayerController;
		if (GetOwningPlayer()->GetCharacter()) SetupDelegate(nullptr, GetOwningPlayer()->GetCharacter());
		else GetOwningPlayer()->OnPossessedPawnChanged.AddDynamic(this, &URoundCountdownTimer::SetupDelegate);
	}
}

void URoundCountdownTimer::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	TagPlayerController = TagPlayerController == nullptr ? Cast<ATagPlayerController>(GetOwningPlayer()) : TagPlayerController;
	if (TagPlayerController)
	{
		const float RoundTimeLeft = GetWorld()->GetTimeSeconds() - TagPlayerController->GetRoundStartTime();
		CountdownText->SetText(FText::FromString(FString::SanitizeFloat(RoundTimeLeft)));
	}
}

void URoundCountdownTimer::SetupDelegate(APawn* OldPawn, APawn* NewPawn)
{
}

