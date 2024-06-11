// Fill out your copyright notice in the Description page of Project Settings.


#include "TagAbility.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Tag/Character/TagCharacter.h"
#include "Tag/GameModes/TagGameMode.h"

UTagAbility::UTagAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
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
			const float AnimResult = TagCharacter->PlayAnimMontage(TagMontage, 4.f);
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
	TaggingCharacter->ReportTag(TaggingCharacter, TagHitCharacter);
	return;
	if (Tag(TagHitCharacter))
	{
		RemoveTagEffect(TaggingCharacter);
		TaggingCharacter->ReportTag(TaggingCharacter, TagHitCharacter);
	}
}

void UTagAbility::RemoveTagEffect(ATagCharacter* TagCharacter)
{
	if (UAbilitySystemComponent* AbilitySystemComponent = TagCharacter->GetAbilitySystemComponent())
	{
		FGameplayTagContainer Tags = FGameplayTagContainer::EmptyContainer;
		Tags.AddTag(FGameplayTag::RequestGameplayTag(FName("Effect.Tagged")));
		const FGameplayEffectQuery TagEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAllOwningTags(Tags);
		AbilitySystemComponent->RemoveActiveEffects(TagEffectQuery, -1);
		
		//Apply speed boost when player tags another player		
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		if (SpeedBoostEffectClass)
		{
			if (const FGameplayEffectSpecHandle SpeedBoostHandle = AbilitySystemComponent->MakeOutgoingSpec(SpeedBoostEffectClass, 0, EffectContext); SpeedBoostHandle.IsValid())
			{
				AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*SpeedBoostHandle.Data.Get(), AbilitySystemComponent);
			}
		}
	}
}

bool UTagAbility::Tag(ATagCharacter* CharacterToTag)
{
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
					AbilitySystemComponent->AddGameplayCue(FGameplayTag::RequestGameplayTag(FName("GameplayCue.Tagged")), EffectContext);
					//Temporarily disable tag ability when player is tagged
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
