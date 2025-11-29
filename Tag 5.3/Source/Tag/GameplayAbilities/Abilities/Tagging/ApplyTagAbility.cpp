// Fill out your copyright notice in the Description page of Project Settings.


#include "ApplyTagAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "LevelInstance/LevelInstanceTypes.h"
#include "Tag/TagGameplayTags.h"
#include "Tag/Character/TagCharacter.h"
#include "Tag/GameModes/TagGameMode.h"

UApplyTagAbility::UApplyTagAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;

	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.ApplyTag")));

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = TagGameplayTags::Event_Tag;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);
}

void UApplyTagAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	SendNotifies(TriggerEventData);
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

void UApplyTagAbility::SendNotifies(const FGameplayEventData* TriggerEventData)
{
	ATagCharacter* TaggingCharacter = Cast<ATagCharacter>(TriggerEventData->Instigator);
	ATagCharacter* TaggedCharacter = Cast<ATagCharacter>(TriggerEventData->Target);
	
	if (ATagGameMode* TagGameMode = GetWorld()->GetAuthGameMode<ATagGameMode>())
	{
		TagGameMode->PlayerTagged(TaggingCharacter, TaggedCharacter);
	}
	
	if (TaggingCharacter)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			TaggingCharacter,
			TagGameplayTags::Event_TagGiven,
			*TriggerEventData
		);
	}

	if (TaggedCharacter)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			TaggedCharacter,
			TagGameplayTags::Event_TagReceived,
			*TriggerEventData
		);
	}
}


