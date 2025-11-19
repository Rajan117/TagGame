// Fill out your copyright notice in the Description page of Project Settings.


#include "OnEventAbility.h"

#include "AbilitySystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UOnEventAbility::UOnEventAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted; 
}

void UOnEventAbility::OnGiveAbility(
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec)
{
	// FAbilityTriggerData TriggerData;
	// TriggerData.TriggerTag = EventTag;
	// TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	// AbilityTriggers.Add(TriggerData);
	
	Super::OnGiveAbility(ActorInfo, Spec);
}

void UOnEventAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	ApplyEffects();
	RemoveEffects();

	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

void UOnEventAbility::ApplyEffects()
{
	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	if (!AbilitySystemComponent) return;

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);
	for (const TSubclassOf<UGameplayEffect>& EffectClass : EffectsToAdd)
	{
		if (EffectClass)
		{
			UKismetSystemLibrary::PrintString(this, TEXT("Applying Effect"), true, true, FLinearColor::Green, 2.f);
			
			if (const FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
				EffectClass,
				0,
				EffectContext);
				EffectSpecHandle.IsValid()
			)
			{
				AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(
					*EffectSpecHandle.Data.Get(),
					AbilitySystemComponent
				);
			}
		}
	}
}

void UOnEventAbility::RemoveEffects()
{
	UKismetSystemLibrary::PrintString(this, TEXT("Removing Effects"), true, true, FLinearColor::Red, 2.f);
	
	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	if (!AbilitySystemComponent) return;

	FGameplayTagContainer Tags;
	for (const FGameplayTag& TagToRemove : EffectsToRemove)
	{
		Tags.AddTag(TagToRemove);
	}
	
	const FGameplayEffectQuery TagEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(Tags);
	AbilitySystemComponent->RemoveActiveEffects(TagEffectQuery, -1);
}


