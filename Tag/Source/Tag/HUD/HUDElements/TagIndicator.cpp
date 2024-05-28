// Fill out your copyright notice in the Description page of Project Settings.


#include "TagIndicator.h"

#include "Kismet/KismetSystemLibrary.h"
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

		if (!TagCharacter->GetIsTagged()) SetRenderOpacity(0.f);
	}
}

void UTagIndicator::UpdateTagIndicator(const TArray<AActor*>& UpdatedActors)
{
	UKismetSystemLibrary::PrintString(this, "Actors perceived...");
	
	if (!TagCharacter || !PerceptionComponent) 	SetRenderOpacity(0.f);
	
	if (!TagCharacter->GetIsTagged() || UpdatedActors.IsEmpty())
	{
		SetRenderOpacity(0.f);
	}
	else
	{
		SetRenderOpacity(1.f);
	}
}
