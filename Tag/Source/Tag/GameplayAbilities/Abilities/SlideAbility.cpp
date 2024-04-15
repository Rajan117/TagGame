// Fill out your copyright notice in the Description page of Project Settings.


#include "SlideAbility.h"

#include "Tag/Character/TagCharacter.h"
#include "Tag/Components/TagCharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

USlideAbility::USlideAbility()
{
	AbilityInputID = EAbilityInput::Slide;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Slide")));
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Sprint")));
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
				UKismetSystemLibrary::PrintString(this, "Entering Slide");
				TagCMC->EnterSlide();
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

	if (const ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get()))
	{
		if (UTagCharacterMovementComponent* TagCMC = Cast<UTagCharacterMovementComponent>(Character->GetCharacterMovement()))
		{
			if (!TagCMC->CanSlide())
			{
				return false;
			}
		}
	}

	return true;
}

void USlideAbility::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ActorInfo != nullptr && ActorInfo->AvatarActor != nullptr)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}
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
}

void USlideAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (const ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get()))
	{
		if (UTagCharacterMovementComponent* TagCMC = Cast<UTagCharacterMovementComponent>(Character->GetCharacterMovement()))
		{
			if (bWasCancelled)
			{
				UKismetSystemLibrary::PrintString(this, "Cancelling Slide");
				TagCMC->ExitSlide();
			}
		}
	}
}
