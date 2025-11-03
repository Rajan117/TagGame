// Fill out your copyright notice in the Description page of Project Settings.


#include "ApplyTagAbility.h"

UApplyTagAbility::UApplyTagAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.ApplyTag")));

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = FGameplayTag::RequestGameplayTag(FName("Event.ChooseTagger"));
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);
}
