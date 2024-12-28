// Fill out your copyright notice in the Description page of Project Settings.


#include "EliminationStatusWidget.h"

#include "Components/TextBlock.h"
#include "Tag/GameStates/TagGameState.h"
#include "Tag/PlayerState/TagPlayerState.h"

void UEliminationStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TagGameState = Cast<ATagGameState>(GetWorld()->GetGameState());
	if (TagGameState)
	{
		TagGameState->OnPlayerEliminatedDelegate.AddDynamic(this, &UEliminationStatusWidget::OnPlayerEliminated);
	}
}

void UEliminationStatusWidget::OnPlayerEliminated(ATagPlayerState* EliminatedPlayer)
{
	if (GetOwningPlayerState<ATagPlayerState>()==EliminatedPlayer)
	{
		StatusText->SetText(FText::FromString("Eliminated"));
	}
}
