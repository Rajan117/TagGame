// Fill out your copyright notice in the Description page of Project Settings.


#include "TagIndicator.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "Tag/Character/TagCharacter.h"

void UTagIndicator::NativeConstruct()
{
	Super::NativeConstruct();

	SetRenderOpacity(0.f);

	if (GetOwningPlayer())
	{
		if (GetOwningPlayer()->GetCharacter()) SetupDelegate(nullptr, GetOwningPlayer()->GetCharacter());
		else GetOwningPlayer()->OnPossessedPawnChanged.AddDynamic(this, &UTagIndicator::SetupDelegate);
	}
}

void UTagIndicator::SetupDelegate(APawn* OldPawn, APawn* NewPawn)
{
	TagCharacter = Cast<ATagCharacter>(NewPawn);
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
	SetRenderOpacity(0.f);
	if (!TagCharacter || !PerceptionComponent)	return;
	
	if (!TagCharacter->GetIsTagged() || !Stimulus.WasSuccessfullySensed())
	{
		SetRenderOpacity(0.f);
	}
	else
	{
		SetRenderOpacity(1.f);
	}
}
