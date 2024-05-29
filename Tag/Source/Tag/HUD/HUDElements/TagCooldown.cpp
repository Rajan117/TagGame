// Fill out your copyright notice in the Description page of Project Settings.


#include "TagCooldown.h"

#include "Components/ProgressBar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tag/Character/TagCharacter.h"

void UTagCooldown::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (GetOwningPlayer())
	{
		if (GetOwningPlayer()->GetCharacter()) SetupDelegate(nullptr, GetOwningPlayer()->GetCharacter());
		else GetOwningPlayer()->OnPossessedPawnChanged.AddDynamic(this, &UTagCooldown::SetupDelegate);
	}
}

void UTagCooldown::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	CooldownProgressBar->SetPercent(GetCoolDownPercentage());
}

void UTagCooldown::SetupDelegate(APawn* OldPawn, APawn* NewPawn)
{
	TagCharacter = Cast<ATagCharacter>(NewPawn);
	if (TagCharacter)
	{
		AbilitySystemComponent = TagCharacter->GetAbilitySystemComponent();
		if (AbilitySystemComponent)
		{
			AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("Effect.TagCooldown")),
				EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UTagCooldown::OnCooldownRemovedCallback);
		}
	}
}

void UTagCooldown::OnCooldownRemovedCallback(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount>0) SetRenderOpacity(1.f);
	else  SetRenderOpacity(0.f);
}

float UTagCooldown::GetCoolDownPercentage() const
{
	if (!AbilitySystemComponent) return 0.f;
	const FGameplayTagContainer CooldownTags = FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("Effect.TagCooldown")));
	float TimeRemaining = 0.f;
	float CooldownDuration = 0.f;
	
	FGameplayEffectQuery const Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTags);
	TArray< TPair<float, float> > DurationAndTimeRemaining = AbilitySystemComponent->GetActiveEffectsTimeRemainingAndDuration(Query);
	if (DurationAndTimeRemaining.Num() > 0)
	{
		int32 BestIdx = 0;
		float LongestTime = DurationAndTimeRemaining[0].Key;
		for (int32 Idx = 1; Idx < DurationAndTimeRemaining.Num(); ++Idx)
		{
			if (DurationAndTimeRemaining[Idx].Key > LongestTime)
			{
				LongestTime = DurationAndTimeRemaining[Idx].Key;
				BestIdx = Idx;
			}
		}

		TimeRemaining = DurationAndTimeRemaining[BestIdx].Key;
		CooldownDuration = DurationAndTimeRemaining[BestIdx].Value;
	}

	return UKismetMathLibrary::SafeDivide(TimeRemaining, CooldownDuration);
}


