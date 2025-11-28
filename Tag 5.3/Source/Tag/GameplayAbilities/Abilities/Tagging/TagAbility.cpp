// Fill out your copyright notice in the Description page of Project Settings.


#include "TagAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbilityTargetDataFilter.h"
#include "Abilities/GameplayAbilityWorldReticle.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"

#include "Tag/GameplayAbilities/GameplayTagLibrary.h"
#include "Tag/Character/TagCharacter.h"
#include "Tag/GameModes/TagGameMode.h"
#include "Tag/GameplayAbilities/GameplayAbilityTasks/GAT_WaitTargetDataUsingActor.h"
#include "Tag/GameplayAbilities/TargetActors/GATA_SphereTrace.h"
#include "Tag/GameplayAbilities/TargetActors/TargetFilters/TagTargetFilter.h"

UTagAbility::UTagAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
	
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Tag")));
	TaggedGameplayCueTag = FGameplayTag::RequestGameplayTag(FName("GameplayCue.Tagged"));
	AimingTag = FGameplayTag::RequestGameplayTag("Equipment.Gun.Aiming");
	AimingRemovalTag = FGameplayTag::RequestGameplayTag("Equipment.Gun.AimingRemoval");

	ActivationRequiredTags.AddTag(UGameplayTagLibrary::TaggedStateTag);
}

void UTagAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
	
	if (ATagCharacter* TagCharacter = CastChecked<ATagCharacter>(ActorInfo->AvatarActor.Get()))
	{
		SphereTraceTargetActor = TagCharacter->GetSphereTraceTargetActor();
		const float AnimResult = TagCharacter->PlayAnimMontage(TagMontage, 4.f);
		TryTag();
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

void UTagAbility::AttemptTag(ATagCharacter* TaggingCharacter, ATagCharacter* TagHitCharacter)
{
	FGameplayEventData EventData;
	EventData.Instigator = TaggingCharacter;
	EventData.Target = TagHitCharacter;
	EventData.EventTag = UGameplayTagLibrary::TagEventTag;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		TaggingCharacter,
		UGameplayTagLibrary::TagEventTag,
		EventData
	);
}

void UTagAbility::TryTag()
{
	if (CommitAbilityCooldown(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true))
	{
		FGameplayAbilityTargetingLocationInfo TraceStartLocation;
		TraceStartLocation.LocationType = EGameplayAbilityTargetingLocationType::ActorTransform;
		TraceStartLocation.SourceActor = GetAvatarActorFromActorInfo();

		FCollisionProfileName TraceProfile(FName("OverlapAllDynamic"));
		FWorldReticleParameters ReticleParams;
		FTagTargetFilter TargetFilter;
		
		FGameplayTargetDataFilter* NewFilter = new FTagTargetFilter(TargetFilter);
		NewFilter->InitializeFilterContext(GetAvatarActorFromActorInfo());
		NewFilter->RequiredActorClass = ATagCharacter::StaticClass();

		FGameplayTargetDataFilterHandle FilterHandle;
		FilterHandle.Filter = TSharedPtr<FGameplayTargetDataFilter>(NewFilter);

		SphereTraceTargetActor->Configure(
              			TraceStartLocation,
              			AimingTag,
              			AimingRemovalTag,
              			TraceProfile,
              			FilterHandle,
              			nullptr,
              			ReticleParams,
              			false,
              			true,
              			true,
              			false,
              			true,
              			true,
              			false,
              			TagRange,
              			TagRadius,
              			false
              			);
	}
	else
	{
		CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
	}
	
	UGAT_WaitTargetDataUsingActor* WaitTargetData = UGAT_WaitTargetDataUsingActor::WaitTargetDataWithReusableActor(
		this,
		FName(),
		EGameplayTargetingConfirmation::Instant,
		SphereTraceTargetActor,
		true
	);
	WaitTargetData->ValidData.AddDynamic(this, &ThisClass::OnTargetDataReady);
	WaitTargetData->ReadyForActivation();
}


void UTagAbility::OnTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetData)
{
	if (CommitAbilityCost(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo()))
	{
		ATagCharacter* TagCharacter = CastChecked<ATagCharacter>(GetAvatarActorFromActorInfo());
		
		for (const TSharedPtr<FGameplayAbilityTargetData> Data : TargetData.Data)
		{
			if (Data->GetHitResult())
			{
				const FGameplayAbilityTargetData* Target = Data.Get();
				if (AActor* TargetActor = Target->GetHitResult()->GetActor())
				{
					if (ATagCharacter* TagHitCharacter = Cast<ATagCharacter>(TargetActor))
					{
						AttemptTag(TagCharacter, TagHitCharacter);
					}
				}
			}
		}
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
	}
	else
	{
		CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
	}
}
