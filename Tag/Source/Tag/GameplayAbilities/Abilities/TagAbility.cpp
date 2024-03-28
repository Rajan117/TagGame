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
			if (ATagCharacter* HitActorTagCharacter = Cast<ATagCharacter>(CheckTag(TagCharacter)))
			{
				AttemptTag(TagCharacter, HitActorTagCharacter);
			}
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

AActor* UTagAbility::CheckTag(ATagCharacter* TagCharacter)
{
	if (!TagCharacter->FPSCameraComponent || !TagCharacter->GetWorld()) return nullptr;
	UKismetSystemLibrary::PrintString(this, FString("Tag"));

	FHitResult TagHitResult;
	FVector Start = TagCharacter->FPSCameraComponent->GetComponentLocation();
	Start.Z -= 10.0f; 
	FVector End = Start + TagCharacter->GetViewRotation().Vector() * 100;
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(TagCharacter);
	
	bool bHit = TagCharacter->GetWorld()->SweepSingleByChannel(
		TagHitResult,
		Start,
		End,
		FQuat::Identity,
		ECollisionChannel::ECC_Pawn,
		FCollisionShape::MakeSphere(10),
		Params
	);
	
	DrawDebugLine(
		TagCharacter->GetWorld(),
		Start,
		End,
		bHit ? FColor::Green : FColor::Red,
		false,
		4.0f,
		0,
		2
	);

	return TagHitResult.GetActor();
}

void UTagAbility::AttemptTag(ATagCharacter* TaggingCharacter, ATagCharacter* TagHitCharacter)
{
	if (UAbilitySystemComponent* AbilitySystemComponent = TagHitCharacter->GetAbilitySystemComponent())
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		if (TagEffectClass)
		{
			FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(TagEffectClass, 0, EffectContext);
			if (NewHandle.IsValid())
			{
				FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);

				if (ActiveGEHandle.WasSuccessfullyApplied())
				{
					RemoveTagEffect(TaggingCharacter);
				}
			}
		}
	}
}

void UTagAbility::RemoveTagEffect(ATagCharacter* TagCharacter)
{
	if (UAbilitySystemComponent* AbilitySystemComponent = TagCharacter->GetAbilitySystemComponent())
	{
		FGameplayTagContainer Tags = FGameplayTagContainer::EmptyContainer;
		Tags.AddTag(FGameplayTag::RequestGameplayTag(FName("EffectTagged")));
		const FGameplayEffectQuery TagEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAllOwningTags(Tags);
		AbilitySystemComponent->RemoveActiveEffects(TagEffectQuery, -1);
	}
}
