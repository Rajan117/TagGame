// Fill out your copyright notice in the Description page of Project Settings.


#include "TagAbility.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Tag/Character/TagCharacter.h"

UTagAbility::UTagAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Tag")));
}

void UTagAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		}
		if (ATagCharacter* TagCharacter = CastChecked<ATagCharacter>(ActorInfo->AvatarActor.Get()))
		{
			AttemptTag(TagCharacter);
		}
	}
}

void UTagAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UTagAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UTagAbility::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ActorInfo != nullptr && ActorInfo->AvatarActor != nullptr)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}
}

void UTagAbility::AttemptTag(ATagCharacter* TagCharacter)
{
	if (!TagCharacter->FPSCameraComponent || !GetWorld()) return;

	FHitResult TagHitResult;
	FVector Start = TagCharacter->FPSCameraComponent->GetComponentLocation();
	Start.Z -= 10.0f; 
	FVector End = Start + TagCharacter->GetViewRotation().Vector() * 40;
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(TagCharacter);
	
	bool bHit = GetWorld()->SweepSingleByChannel(
		TagHitResult,
		Start,
		End,
		FQuat::Identity,
		ECollisionChannel::ECC_Pawn,
		FCollisionShape::MakeSphere(10), // Specify the radius of the sphere
		Params
	);
	
	DrawDebugLine(
		GetWorld(),
		Start,
		End,
		bHit ? FColor::Green : FColor::Red,
		false,
		4.0f,
		0,
		2
	);
}
