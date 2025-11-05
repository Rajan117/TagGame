// Fill out your copyright notice in the Description page of Project Settings.


#include "ApplyTagAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "LevelInstance/LevelInstanceTypes.h"
#include "Tag/Character/TagCharacter.h"
#include "Tag/GameModes/TagGameMode.h"
#include "Tag/GameplayAbilities/GameplayTagLibrary.h"

UApplyTagAbility::UApplyTagAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.ApplyTag")));

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = UGameplayTagLibrary::TagEventTag;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);
}

void UApplyTagAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		SendNotifies(TriggerEventData);
	}
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
			UGameplayTagLibrary::TagGivenEventTag,
			*TriggerEventData
		);
	}

	if (TaggedCharacter)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			TaggedCharacter,
			UGameplayTagLibrary::TagReceivedEventTag,
			*TriggerEventData
		);
	}
}


