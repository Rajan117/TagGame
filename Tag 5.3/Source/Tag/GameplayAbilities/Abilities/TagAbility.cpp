// Fill out your copyright notice in the Description page of Project Settings.


#include "TagAbility.h"

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
	
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Tag")));
	TaggedGameplayCueTag = FGameplayTag::RequestGameplayTag(FName("GameplayCue.Tagged"));
	AimingTag = FGameplayTag::RequestGameplayTag("Equipment.Gun.Aiming");
	AimingRemovalTag = FGameplayTag::RequestGameplayTag("Equipment.Gun.AimingRemoval");

	
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
			SphereTraceTargetActor = TagCharacter->GetSphereTraceTargetActor();
			const float AnimResult = TagCharacter->PlayAnimMontage(TagMontage, 4.f);
			TryTag();
			return;
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

AActor* UTagAbility::CheckTag(const ATagCharacter* TagCharacter) const
{
	if (const UAIPerceptionComponent* PerceptionComponent = TagCharacter->GetPerceptionComponent())
	{
		TArray<AActor*> OutActors;
		PerceptionComponent->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), OutActors);

		for (AActor* Actor : OutActors)
		{
			if (ATagCharacter* TagActor = Cast<ATagCharacter>(Actor); !TagActor->GetIsTagged())
			{
				return TagActor;
			}
		}
	}
	return nullptr;
}

void UTagAbility::AttemptTag(ATagCharacter* TaggingCharacter, ATagCharacter* TagHitCharacter)
{
	if (Tag(TagHitCharacter))
	{
		RemoveTagEffect(TaggingCharacter);
		if (ATagGameMode* TagGameMode = GetWorld()->GetAuthGameMode<ATagGameMode>())
		{
			TagGameMode->PlayerTagged(TaggingCharacter, TagHitCharacter);
		}
	}
}

void UTagAbility::RemoveTagEffect(ATagCharacter* TagCharacter)
{
	if (UAbilitySystemComponent* AbilitySystemComponent = TagCharacter->GetAbilitySystemComponent())
	{
		FGameplayTagContainer Tags;
		Tags.AddTag(UGameplayTagLibrary::TaggedStateTag);
		const FGameplayEffectQuery TagEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAllOwningTags(Tags);
		AbilitySystemComponent->RemoveActiveEffects(TagEffectQuery, -1);
		
		//Apply speed boost when player tags another player		
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		if (SpeedBoostEffectClass)
		{
			if (const FGameplayEffectSpecHandle SpeedBoostHandle = AbilitySystemComponent->MakeOutgoingSpec(
				SpeedBoostEffectClass,
				0,
				EffectContext);
				SpeedBoostHandle.IsValid()
			)
			{
				AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(
					*SpeedBoostHandle.Data.Get(),
					AbilitySystemComponent
				);
			}
		}
	}
}

bool UTagAbility::Tag(ATagCharacter* CharacterToTag)
{
	UKismetSystemLibrary::PrintString(this, TEXT("Tagging"));
	if (UAbilitySystemComponent* AbilitySystemComponent = CharacterToTag->GetAbilitySystemComponent())
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		if (TagEffectClass)
		{
			if (const FGameplayEffectSpecHandle TaggedHandle = AbilitySystemComponent->MakeOutgoingSpec(TagEffectClass, 0, EffectContext); TaggedHandle.IsValid())
			{
				if (AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*TaggedHandle.Data.Get(), AbilitySystemComponent).WasSuccessfullyApplied())
				{
					AbilitySystemComponent->AddGameplayCue(TaggedGameplayCueTag, EffectContext);
					if (TagDisabledEffectClass)
					{
						if (const FGameplayEffectSpecHandle TaggedDebuffHandle = AbilitySystemComponent->MakeOutgoingSpec(TagDisabledEffectClass, 0, EffectContext); TaggedDebuffHandle.IsValid())
						{
							AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*TaggedDebuffHandle.Data.Get(), AbilitySystemComponent);
						}
					}
					return true;
				}
			}
		}
	}
	return false;
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
	UKismetSystemLibrary::PrintString(this, TEXT("OnTargetDataReady called"));
	if (CommitAbilityCost(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo()))
	{
		ATagCharacter* TagCharacter = CastChecked<ATagCharacter>(GetAvatarActorFromActorInfo());
		
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
					UKismetSystemLibrary::PrintString(this, TargetActor->GetName()); // Crashes
					if (ATagCharacter* TagHitCharacter = Cast<ATagCharacter>(TargetActor))
					{
						AttemptTag(TagCharacter, TagHitCharacter);
					}
				}
				else
				{
					UKismetSystemLibrary::PrintString(this, TEXT("TargetActor is null"));
				}
			}
		}

		if (TargetData.Data.Num() == 0)
		{
			UKismetSystemLibrary::PrintString(this, TEXT("No valid target data found"));
		}

		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
	}
	else
	{
		CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
	}
}
