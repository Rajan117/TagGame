// Fill out your copyright notice in the Description page of Project Settings.


#include "TagVingette.h"

#include "Tag/Character/TagCharacter.h"

void UTagVingette::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetRenderOpacity(0.f);
	if (GetOwningPlayer())
	{
		if (GetOwningPlayer()->GetCharacter()) SetupDelegate(nullptr, GetOwningPlayer()->GetCharacter());
		else GetOwningPlayer()->OnPossessedPawnChanged.AddDynamic(this, &UTagVingette::SetupDelegate);
	}
}

void UTagVingette::SetupDelegate(APawn* OldPawn, APawn* NewPawn)
{
	TagCharacter = Cast<ATagCharacter>(NewPawn);
	if (TagCharacter)
	{
		TagCharacter->OnTagStateChangedDelegate.AddDynamic(this, &UTagVingette::OnTagStateChanged);
	}
}

void UTagVingette::OnTagStateChanged(bool bIsTagged)
{
	if (bIsTagged) 	SetRenderOpacity(1.f);
	else SetRenderOpacity(0.f);
}
