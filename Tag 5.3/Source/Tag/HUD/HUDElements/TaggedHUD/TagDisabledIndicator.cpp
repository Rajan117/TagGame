// Fill out your copyright notice in the Description page of Project Settings.


#include "TagDisabledIndicator.h"
#include "Tag/Character/TagCharacter.h"

void UTagDisabledIndicator::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Hidden);
	if (GetOwningPlayer())
	{
		if (GetOwningPlayer()->GetCharacter()) SetupDelegate(nullptr, GetOwningPlayer()->GetCharacter());
		else GetOwningPlayer()->OnPossessedPawnChanged.AddDynamic(this, &UTagDisabledIndicator::SetupDelegate);
	}
}

void UTagDisabledIndicator::SetupDelegate(APawn* OldPawn, APawn* NewPawn)
{
	TagCharacter = Cast<ATagCharacter>(NewPawn);
	if (TagCharacter)
	{
		AbilitySystemComponent = TagCharacter->GetAbilitySystemComponent();
		if (AbilitySystemComponent)
		{
			AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("Ability.Tag.Disabled")),
				EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UTagDisabledIndicator::OnTagDisabled);
		}
	}
}

void UTagDisabledIndicator::OnTagDisabled(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0) SetVisibility(ESlateVisibility::Visible);
	else SetVisibility(ESlateVisibility::Hidden);
}
