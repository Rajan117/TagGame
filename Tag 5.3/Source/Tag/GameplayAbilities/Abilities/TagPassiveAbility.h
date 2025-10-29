// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EIGameplayAbility.h"
#include "TagPassiveAbility.generated.h"

class ATagCharacter;
class UGAT_WaitTargetDataUsingActor;
class AGATA_SphereTrace;
/**
 * 
 */
UCLASS()
class TAG_API UTagPassiveAbility : public UEIGameplayAbility
{
	GENERATED_BODY()

public:
	UTagPassiveAbility();

protected:
	
	virtual void ActivateAbility(
		FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
	    FGameplayAbilityActivationInfo ActivationInfo,
	    const FGameplayEventData* TriggerEventData
	) override;

	UFUNCTION()
	void OnTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetData);

	virtual void OnGiveAbility(
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilitySpec& Spec
	) override;

	// Ensure we clean up when the ability ends
	virtual void EndAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	// Helper to schedule the next ConfirmTargetingAndContinue on the next tick (avoids recursive stack overflow)
	void ScheduleConfirmTargetingNextTick();
	void DoConfirmTargeting();

	void UpdateCouldTagSomeoneState(bool bCouldTagSomeone);

	UPROPERTY(EditDefaultsOnly)
	float TagRange = 150.f;
	UPROPERTY(EditDefaultsOnly)
	float TagRadius = 75.f;
	UPROPERTY(EditDefaultsOnly)
	float CheckTagRate = 0.005f;

private:
	UPROPERTY()
	ATagCharacter* TagCharacter;
	UPROPERTY()
	UAbilitySystemComponent* TagCharacterAbilitySystemComponent;
	UPROPERTY()
	AGATA_SphereTrace* SphereTraceTargetActor;
	UPROPERTY()
	UGAT_WaitTargetDataUsingActor* WaitTargetData;
	
	FTimerHandle ConfirmTimerHandle;
	
};
