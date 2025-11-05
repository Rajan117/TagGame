// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tag/GameplayAbilities/Abilities/EIGameplayAbility.h"
#include "OnTagReceivedAbility.generated.h"

/**
 * 
 */
UCLASS()
class TAG_API UOnTagReceivedAbility : public UEIGameplayAbility
{
	GENERATED_BODY()
public:
	UOnTagReceivedAbility();
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void HandleTagReceived(const FGameplayEventData* TriggerEventData);

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TArray<TSubclassOf<class UGameplayEffect>> EffectsToAdd;
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TArray<TSubclassOf<class UGameplayEffect>> EffectsToRemove;

private:
	
};
