// Fill out your copyright notice in the Description page of Project Settings.


#include "TagIndicator.h"

#include "Perception/AIPerceptionComponent.h"
#include "Tag/Character/TagCharacter.h"

void UTagIndicator::NativeConstruct()
{
	Super::NativeConstruct();

	if (!GetOwningPlayer()) return;

	TagCharacter = Cast<ATagCharacter>(GetOwningPlayer()->GetCharacter());
	if (TagCharacter)
	{
		PerceptionComponent = TagCharacter->GetPerceptionComponent();
		if (PerceptionComponent)
		{
			PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &ThisClass::UpdateTagIndicator);
		}

		if (!TagCharacter->GetIsTagged()) SetVisibility(ESlateVisibility::Hidden);
	}
}

void UTagIndicator::UpdateTagIndicator(const TArray<AActor*>& UpdatedActors)
{
	if (!TagCharacter || !PerceptionComponent) 	SetVisibility(ESlateVisibility::Hidden);
	
	if (!TagCharacter->GetIsTagged() || UpdatedActors.IsEmpty())
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		SetVisibility(ESlateVisibility::Visible);
	}
}
