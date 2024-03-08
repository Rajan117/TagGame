// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityInput.h"
#include "Abilities/GameplayAbility.h"
#include "EIGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class TAG_API UEIGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ability")
	EAbilityInput AbilityInputID = EAbilityInput::None;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ability")
	EAbilityInput AbilityID = EAbilityInput::None;
};
