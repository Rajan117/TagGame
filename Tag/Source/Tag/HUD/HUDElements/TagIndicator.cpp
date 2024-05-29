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
		if (GetOwningPlayer()->GetCharacter()) SetupDelegate(GetOwningPlayer()->GetCharacter());
		else GetOwningPlayer()->OnPossessedPawnChanged.AddDynamic(this, &UTagIndicator::OnPosessed);
	}
}

void UTagIndicator::OnPosessed(APawn* OldPawn, APawn* NewPawn)
{
	UKismetSystemLibrary::PrintString(this, "Pawn Changed Delegate");
	SetupDelegate(NewPawn);
}

void UTagIndicator::SetupDelegate(APawn* InPawn)
{
	TagCharacter = Cast<ATagCharacter>(InPawn);
	if (TagCharacter)
	{
		PerceptionComponent = TagCharacter->GetPerceptionComponent();
		if (PerceptionComponent)
		{
			PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &UTagIndicator::UpdateTagIndicator);
			UKismetSystemLibrary::PrintString(this, "Perception Bound");
		}
	}
}

void UTagIndicator::UpdateTagIndicator(AActor* Actor, FAIStimulus Stimulus)
{
	UKismetSystemLibrary::PrintString(this, "Updating Tag Indicator");

	SetRenderOpacity(0.f);
	if (!TagCharacter || !PerceptionComponent)	return;
	
	if (!TagCharacter->GetIsTagged() || !Stimulus.WasSuccessfullySensed())
	{
		UKismetSystemLibrary::PrintString(this, "No Tag Target");
		SetRenderOpacity(0.f);
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, "Potential Tag Target");
		SetRenderOpacity(1.f);
	}
}
