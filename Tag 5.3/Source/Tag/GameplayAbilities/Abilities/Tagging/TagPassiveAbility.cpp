// Fill out your copyright notice in the Description page of Project Settings.


#include "TagPassiveAbility.h"

#include "Tag/Character/TagCharacter.h"
#include "Tag/GameplayAbilities/GameplayTagLibrary.h"
#include "Tag/GameplayAbilities/GameplayAbilityTasks/GAT_WaitTargetDataUsingActor.h"
#include "Tag/GameplayAbilities/TargetActors/GATA_SphereTrace.h"
#include "Tag/GameplayAbilities/TargetActors/TargetFilters/TagTargetFilter.h"
#include "TimerManager.h"

UTagPassiveAbility::UTagPassiveAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.TagPassive")));

	SphereTraceTargetActor = nullptr;
	WaitTargetData = nullptr;
	TagCharacter = nullptr;
}

void UTagPassiveAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	TagCharacter = CastChecked<ATagCharacter>(ActorInfo->AvatarActor.Get());
	if (TagCharacter)
	{
		TagCharacterAbilitySystemComponent = TagCharacter->GetAbilitySystemComponent();
	
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
		NewFilter->SelfActor = TagCharacter;
		NewFilter->SelfFilter = ETargetDataFilterSelf::TDFS_NoSelf;

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
						  false,
						  false,
						  true,
						  true,
						  false,
						  TagRange,
						  TagRadius,
						  false
		);
		
		WaitTargetData = UGAT_WaitTargetDataUsingActor::WaitTargetDataWithReusableActor(
			this,
			FName(),
			EGameplayTargetingConfirmation::CustomMulti,
			SphereTraceTargetActor,
			true
		);
		WaitTargetData->ValidData.AddDynamic(this, &ThisClass::OnTargetDataReady);
		WaitTargetData->ReadyForActivation();
		WaitTargetData->ExternalConfirm(false);

	}
}

void UTagPassiveAbility::OnTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetData)
{
	bool bCouldTagSomeone = false;
	for (const TSharedPtr<FGameplayAbilityTargetData> Data : TargetData.Data)
	{
		if (Data->GetHitResult() != nullptr)
		{
			const FGameplayAbilityTargetData* Target = Data.Get();
			if (AActor* TargetActor = Target->GetHitResult()->GetActor())
			{
				if (const ATagCharacter* TagHitCharacter = Cast<ATagCharacter>(TargetActor))
				{
					bCouldTagSomeone = true;
				}
			}
		}
	}
	UpdateCouldTagSomeoneState(bCouldTagSomeone);
	ScheduleConfirmTargetingNextTick();
}

void UTagPassiveAbility::ScheduleConfirmTargetingNextTick()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(ConfirmTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(
			ConfirmTimerHandle,
			this,
			&ThisClass::DoConfirmTargeting,
			CheckTagRate,
			false
		);
	}
}

void UTagPassiveAbility::DoConfirmTargeting()
{
	if (SphereTraceTargetActor)
	{
		WaitTargetData->ExternalConfirm(false);
	}
}

void UTagPassiveAbility::UpdateCouldTagSomeoneState(bool bCouldTagSomeone)
{
	if (!TagCharacterAbilitySystemComponent) return;
	const bool bHasTag = TagCharacterAbilitySystemComponent->HasMatchingGameplayTag(UGameplayTagLibrary::CouldTagSomeoneStateTag);
	if (bHasTag == bCouldTagSomeone) return;
	
	if (bCouldTagSomeone)
	{
		TagCharacterAbilitySystemComponent->AddLooseGameplayTag(UGameplayTagLibrary::CouldTagSomeoneStateTag);
	}
	else
	{
		TagCharacterAbilitySystemComponent->RemoveLooseGameplayTag(UGameplayTagLibrary::CouldTagSomeoneStateTag);
	}
}

void UTagPassiveAbility::EndAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(ConfirmTimerHandle);
	}

	if (SphereTraceTargetActor)
	{
		SphereTraceTargetActor->TargetDataReadyDelegate.RemoveAll(this);
		SphereTraceTargetActor->StopTargeting();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UTagPassiveAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
}
