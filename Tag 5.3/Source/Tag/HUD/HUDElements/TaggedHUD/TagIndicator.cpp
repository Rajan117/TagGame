// Fill out your copyright notice in the Description page of Project Settings.


#include "TagIndicator.h"

#include "Tag/TagGameplayTags.h"
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

void UTagIndicator::NativeDestruct()
{
	Super::NativeDestruct();

	CouldTagSomeoneHandle.Reset();
}

void UTagIndicator::SetupDelegate(APawn* OldPawn, APawn* NewPawn)
{
	TagCharacter = Cast<ATagCharacter>(NewPawn);
	if (TagCharacter)
	{
		// TagCharacter->OnCouldTagSomeoneChangedDelegate.AddDynamic(this, &UTagIndicator::UpdateTagIndicator);
		TagCharacterAbilitySystemComponent = TagCharacter->GetAbilitySystemComponent();
		if (TagCharacterAbilitySystemComponent)
		{
			CouldTagSomeoneHandle.Reset();
			CouldTagSomeoneHandle = TagCharacterAbilitySystemComponent->RegisterGameplayTagEvent(
				TagGameplayTags::State_CouldTagSomeone,
				EGameplayTagEventType::NewOrRemoved
			).AddUObject(this, &UTagIndicator::UpdateTagIndicator);
		}
	}
}

void UTagIndicator::UpdateTagIndicator(const FGameplayTag Tag, int32 TagCount)
{
	SetRenderOpacity(0.f);
	if (!TagCharacter) return;
	SetRenderOpacity(TagCount > 0 ? 1.f : 0.f);
}
