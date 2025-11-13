// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tag/GameplayAbilities/Abilities/EIGameplayAbility.h"
#include "OnEventAbility.generated.h"

/**
 *  Generic ability that applies and removes effects when triggered by a specified event.
 */
UCLASS()
class TAG_API UOnEventAbility : public UEIGameplayAbility
{
	GENERATED_BODY()

public:
	UOnEventAbility();

	virtual void PostInitProperties() override;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Event")
	FGameplayTag EventTag;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TArray<TSubclassOf<class UGameplayEffect>> EffectsToAdd;
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TArray<FGameplayTag> EffectsToRemove;

private:
	virtual void ApplyEffects();
	virtual void RemoveEffects();
};
