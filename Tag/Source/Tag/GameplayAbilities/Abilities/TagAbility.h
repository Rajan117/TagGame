// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EIGameplayAbility.h"
#include "Abilities/GameplayAbility.h"
#include "TagAbility.generated.h"

class ATagPlayerController;
class ATagCharacter;

/**
 * 
 */
UCLASS()
class TAG_API UTagAbility : public UEIGameplayAbility
{
	GENERATED_BODY()

public:
	UTagAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateCancelAbility) override;

	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	AActor* CheckTag(ATagCharacter* TagCharacter);
	void AttemptTag(ATagCharacter* TaggingCharacter, ATagCharacter* TagHitCharacter);
	void RemoveTagEffect(ATagCharacter* TagCharacter);
	bool Tag(ATagCharacter* CharacterToTag);

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<class UGameplayEffect> TagEffectClass;
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<class UGameplayEffect> SpeedBoostEffectClass;
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<class UGameplayEffect> TagDisabledEffectClass;

	UPROPERTY(EditDefaultsOnly)
	float TagRange = 150.f;
	UPROPERTY(EditDefaultsOnly)
	float TagRadius = 75.f;
};
