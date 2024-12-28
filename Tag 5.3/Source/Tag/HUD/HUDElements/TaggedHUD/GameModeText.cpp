// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeText.h"

#include "Components/TextBlock.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UGameModeText::NativeConstruct()
{
	Super::NativeConstruct();

	if (GetOwningPlayer())
	{
		if (GetOwningPlayer()->GetCharacter()) UpdateGameModeText(nullptr, GetOwningPlayer()->GetCharacter());
		else GetOwningPlayer()->OnPossessedPawnChanged.AddDynamic(this, &UGameModeText::UpdateGameModeText);
	}
}

void UGameModeText::UpdateGameModeText(APawn* OldPawn, APawn* NewPawn)
{
	if (!GetWorld()) return;
	if (const AGameStateBase* GameStateBase = GetWorld()->GetGameState())
	{
		GameModeTextBlock->SetText(FText::FromString(GameStateBase->GameModeClass->GetName()));
	}
}
