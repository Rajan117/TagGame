// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tag/GameplayAbilities/Abilities/EIGameplayAbility.h"
#include "ApplyTagAbility.generated.h"

/**
 * 
 */
UCLASS()
class TAG_API UApplyTagAbility : public UEIGameplayAbility
{
	GENERATED_BODY()

public:
	UApplyTagAbility();
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void SendNotifies(const FGameplayEventData* TriggerEventData);
};
