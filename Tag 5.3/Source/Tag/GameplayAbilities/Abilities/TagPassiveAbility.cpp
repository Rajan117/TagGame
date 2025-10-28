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
	// ActivationRequiredTags.AddTag(UGameplayTagLibrary::TaggedStateTag);

	// Initialize pointer to avoid uninitialized-member warning
	SphereTraceTargetActor = nullptr;
	// Initialize WaitTargetData pointer
	WaitTargetData = nullptr;

	
}

void UTagPassiveAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			//EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		}

		TagCharacter = CastChecked<ATagCharacter>(ActorInfo->AvatarActor.Get());
		if (TagCharacter)
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

			// SphereTraceTargetActor->TargetDataReadyDelegate.AddUObject(this, &ThisClass::OnTargetDataReady);
			// SphereTraceTargetActor->StartTargeting(this);
			// SphereTraceTargetActor->ConfirmTargetingAndContinue();
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
}

void UTagPassiveAbility::OnTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetData)
{
	UKismetSystemLibrary::PrintString(this, TEXT("OnTargetDataReady called"), true, true, FLinearColor::Green, 5.f);
	bool bCouldTagSomeone = false;
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
					bCouldTagSomeone = true;
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
	TagCharacter->OnCouldTagSomeoneChangedDelegate.Broadcast(bCouldTagSomeone);
	ScheduleConfirmTargetingNextTick();
}

void UTagPassiveAbility::ScheduleConfirmTargetingNextTick()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(ConfirmTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(ConfirmTimerHandle, this, &ThisClass::DoConfirmTargeting, 0.01f, false);
	}
}

void UTagPassiveAbility::DoConfirmTargeting()
{
	if (SphereTraceTargetActor)
	{
		WaitTargetData->ExternalConfirm(false);
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
