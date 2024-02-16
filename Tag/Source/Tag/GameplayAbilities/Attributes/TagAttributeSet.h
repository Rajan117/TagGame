// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AttributeHelpers.h"
#include "TagAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class TAG_API UTagAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData MaxSpeed;
	ATTRIBUTE_ACCESSORS(UTagAttributeSet, MaxSpeed);
};
