// Fill out your copyright notice in the Description page of Project Settings.


#include "TagPassiveAbility.h"

#include "Tag/Character/TagCharacter.h"
#include "Tag/GameplayAbilities/GameplayTagLibrary.h"
#include "Tag/GameplayAbilities/GameplayAbilityTasks/GAT_WaitTargetDataUsingActor.h"
#include "Tag/GameplayAbilities/TargetActors/GATA_SphereTrace.h"
#include "Tag/GameplayAbilities/TargetActors/TargetFilters/TagTargetFilter.h"

UTagPassiveAbility::UTagPassiveAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.TagPassive")));
	ActivationRequiredTags.AddTag(UGameplayTagLibrary::TaggedStateTag);

	// Initialize pointer to avoid uninitialized-member warning
	SphereTraceTargetActor = nullptr;

	
}

void UTagPassiveAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
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
			SphereTraceTargetActor = TagCharacter->GetSphereTraceTargetActor();
			
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
							  FGameplayTag::EmptyTag,
							  FGameplayTag::EmptyTag,
							  TraceProfile,
							  FilterHandle,
							  nullptr,
							  ReticleParams,
							  false,
							  false,
							  true,
							  false,
							  true,
							  true,
							  false,
							  TagRange,
							  TagRadius,
							  false
			);

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
	}
}

void UTagPassiveAbility::OnTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetData)
{
	for (const TSharedPtr<FGameplayAbilityTargetData> Data : TargetData.Data)
	{
		if (Data->GetHitResult() == nullptr)
		{
			UKismetSystemLibrary::PrintString(this, TEXT("Hit Result is null"));
		}
		else
		{
			const FGameplayAbilityTargetData* Target = Data.Get();
			if (AActor* TargetActor = Target->GetHitResult()->GetActor())
			{
				if (const ATagCharacter* TagHitCharacter = Cast<ATagCharacter>(TargetActor))
				{
					UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Valid TagHitCharacter: %s"), *TagHitCharacter->GetName()));
				}
				else
				{
					UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Found Actor (not TagCharacter): %s"), *TargetActor->GetName()));
				}
			}
			else
			{
				UKismetSystemLibrary::PrintString(this, TEXT("TargetActor is null"));
			}
		}
	}
}
