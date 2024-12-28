// Fill out your copyright notice in the Description page of Project Settings.


#include "TaggedHUD.h"

#include "Tag/Character/TagCharacter.h"

void UTaggedHUD::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Hidden);
	if (GetOwningPlayer())
	{
		if (GetOwningPlayer()->GetCharacter()) SetupDelegate(nullptr, GetOwningPlayer()->GetCharacter());
		else GetOwningPlayer()->OnPossessedPawnChanged.AddDynamic(this, &UTaggedHUD::SetupDelegate);
	}
}

void UTaggedHUD::SetupDelegate(APawn* OldPawn, APawn* NewPawn)
{
	TagCharacter = Cast<ATagCharacter>(NewPawn);
	if (TagCharacter)
	{
		TagCharacter->OnTagStateChangedDelegate.AddDynamic(this, &UTaggedHUD::OnTagStateChanged);
	}
}

void UTaggedHUD::OnTagStateChanged(bool bIsTagged)
{
	if (bIsTagged) 	SetVisibility(ESlateVisibility::Visible);
	else SetVisibility(ESlateVisibility::Hidden);
}