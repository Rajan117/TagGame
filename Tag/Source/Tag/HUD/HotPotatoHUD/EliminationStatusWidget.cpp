// Fill out your copyright notice in the Description page of Project Settings.


#include "EliminationStatusWidget.h"

#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Tag/GameStates/TagRoundBasedGameState.h"
#include "Tag/PlayerState/TagPlayerState.h"

void UEliminationStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UKismetSystemLibrary::PrintString(this, "Binding");

	TagRoundBasedGameState = Cast<ATagRoundBasedGameState>(GetWorld()->GetGameState());
	if (TagRoundBasedGameState)
	{
		TagRoundBasedGameState->OnPlayerEliminatedDelegate.AddDynamic(this, &UEliminationStatusWidget::OnPlayerEliminated);
	}
}


void UEliminationStatusWidget::OnPlayerEliminated(ATagPlayerState* EliminatedPlayer)
{
	UKismetSystemLibrary::PrintString(this, "Player Elimed");
	if (GetOwningPlayerState<ATagPlayerState>()==EliminatedPlayer)
	{
		StatusText->SetText(FText::FromString("Eliminated"));
	}
}
