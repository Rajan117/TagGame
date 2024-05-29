// Fill out your copyright notice in the Description page of Project Settings.


#include "TagIndicator.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "Tag/Character/TagCharacter.h"

void UTagIndicator::NativeConstruct()
{
	Super::NativeConstruct();

	SetRenderOpacity(0.f);
	
	if (!GetOwningPlayer()) return;

	TagCharacter = Cast<ATagCharacter>(GetOwningPlayer()->GetCharacter());
	if (TagCharacter)
	{
		PerceptionComponent = TagCharacter->GetPerceptionComponent();
		if (PerceptionComponent)
		{
			PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &UTagIndicator::UpdateTagIndicator);
		}
	}
}

void UTagIndicator::UpdateTagIndicator(AActor* Actor, FAIStimulus Stimulus)
{
	if (!TagCharacter || !PerceptionComponent) 	SetRenderOpacity(0.f);
	if (!TagCharacter->GetIsTagged() || !Stimulus.WasSuccessfullySensed())
	{
		SetRenderOpacity(0.f);
	}
	else
	{
		SetRenderOpacity(1.f);
	}
}
