// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagLibrary.generated.h"

/**
 * 
 */
UCLASS()
class TAG_API UGameplayTagLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// States
	static const FGameplayTag TaggedStateTag;
	static const FGameplayTag CouldTagSomeoneStateTag;
	
	// Effects
	static const FGameplayTag TaggedEffectTag;
	
	// Gameplay Cues
	static const FGameplayTag TaggedGameplayCueTag;
	
	// Events
	static const FGameplayTag ChooseTaggerEventTag;
	static const FGameplayTag TagEventTag;
	static const FGameplayTag TagReceivedEventTag;
	static const FGameplayTag TagGivenEventTag;
};
