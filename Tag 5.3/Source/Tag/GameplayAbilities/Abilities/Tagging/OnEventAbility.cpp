// Fill out your copyright notice in the Description page of Project Settings.


#include "OnEventAbility.h"

#include "AbilitySystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UOnEventAbility::UOnEventAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted; 
}

void UOnEventAbility::PostInitProperties()
{
	Super::PostInitProperties();

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = EventTag;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);
}

void UOnEventAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		UKismetSystemLibrary::PrintString(this, TEXT("OnEventAbility Activated"));
		ApplyEffects();
		RemoveEffects();
	}
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


