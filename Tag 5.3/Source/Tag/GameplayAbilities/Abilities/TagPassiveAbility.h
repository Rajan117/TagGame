// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EIGameplayAbility.h"
#include "TagPassiveAbility.generated.h"

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


	UPROPERTY(EditDefaultsOnly)
	float TagRange = 150.f;
	UPROPERTY(EditDefaultsOnly)
	float TagRadius = 75.f;

private:
	UPROPERTY()
	AGATA_SphereTrace* SphereTraceTargetActor;
	
};
