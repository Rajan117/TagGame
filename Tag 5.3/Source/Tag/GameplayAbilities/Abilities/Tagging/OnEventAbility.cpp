// Fill out your copyright notice in the Description page of Project Settings.


#include "OnEventAbility.h"

#include "AbilitySystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UOnEventAbility::UOnEventAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
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


