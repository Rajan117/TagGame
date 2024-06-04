// Fill out your copyright notice in the Description page of Project Settings.


#include "StaminaBar.h"

#include "Components/ProgressBar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Nodes/InterchangeBaseNode.h"
#include "Tag/Character/TagCharacter.h"

void UStaminaBar::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (GetOwningPlayer())
	{
		if (GetOwningPlayer()->GetCharacter()) SetupDelegate(nullptr, GetOwningPlayer()->GetCharacter());
		else GetOwningPlayer()->OnPossessedPawnChanged.AddDynamic(this, &UStaminaBar::SetupDelegate);
	}
	StaminaProgressBar->SetPercent(1.f);
}

void UStaminaBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const float Percent = UKismetMathLibrary::FInterpTo(
		StaminaProgressBar->GetPercent(),
		TargetPercent,
		InDeltaTime,
		StaminaBarInterpSpeed);
	StaminaProgressBar->SetPercent(Percent);
}

void UStaminaBar::SetupDelegate(APawn* OldPawn, APawn* NewPawn)
{
	TagCharacter = Cast<ATagCharacter>(NewPawn);
	if (TagCharacter && TagCharacter->AbilitySystemComponent && TagCharacter->GetAttributeSet())
	{
		AbilitySystemComponent = TagCharacter->AbilitySystemComponent;
		StandardAttributes = TagCharacter->GetAttributeSet();
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			StandardAttributes->GetStaminaAttribute()).AddUObject(this, &UStaminaBar::StaminaChanged);
	}
}

void UStaminaBar::StaminaChanged(const FOnAttributeChangeData& Data)
{
	TargetPercent = UKismetMathLibrary::SafeDivide(
		Data.NewValue, StandardAttributes->GetMaxStamina());
}

