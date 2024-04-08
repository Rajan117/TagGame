// Fill out your copyright notice in the Description page of Project Settings.


#include "SlideAbility.h"

#include "Tag/Character/TagCharacter.h"
#include "Tag/Components/TagCharacterMovementComponent.h"
#include "GameFramework/Character.h"

USlideAbility::USlideAbility()
{
	AbilityInputID = EAbilityInput::Slide;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Slide")));
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Sprint")));
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Jump")));
}

void USlideAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		}
		if (const ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get()))
		{
			if (UTagCharacterMovementComponent* TagCMC = Cast<UTagCharacterMovementComponent>(Character->GetCharacterMovement()))
			{
				//TagCMC->StartCrouching();
			}
		}
	}
}

bool USlideAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	return true;
}

void USlideAbility::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{

}

void USlideAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (ScopeLockCount > 0)
	{
		WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &USlideAbility::CancelAbility, Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility));
		return;
	}

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
	
	if (const ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get()))
	{
		if (UTagCharacterMovementComponent* TagCMC = Cast<UTagCharacterMovementComponent>(Character->GetCharacterMovement()))
		{
			//TagCMC->StopCrouching();
		}
	}
}
