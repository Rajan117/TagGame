// Fill out your copyright notice in the Description page of Project Settings.


#include "TagAbility.h"

#include "Tag/Character/TagCharacter.h"

UTagAbility::UTagAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Tag")));
}

void UTagAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("Tagging"));

	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		}
		if (ATagCharacter* TagCharacter = CastChecked<ATagCharacter>(ActorInfo->AvatarActor.Get()))
		{
			TagCharacter->Tag();
		}
	}
}
