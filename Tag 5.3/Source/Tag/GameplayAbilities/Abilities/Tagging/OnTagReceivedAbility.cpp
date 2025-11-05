// Fill out your copyright notice in the Description page of Project Settings.


#include "OnTagReceivedAbility.h"

#include "Tag/GameplayAbilities/GameplayTagLibrary.h"

UOnTagReceivedAbility::UOnTagReceivedAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.ApplyTag")));

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = UGameplayTagLibrary::TagReceivedEventTag;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);
}

void UOnTagReceivedAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		
	}
}

void UOnTagReceivedAbility::HandleTagReceived(const FGameplayEventData* TriggerEventData)
{
}
