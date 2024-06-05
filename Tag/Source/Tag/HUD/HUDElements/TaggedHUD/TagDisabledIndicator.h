// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TagDisabledIndicator.generated.h"

class UAbilitySystemComponent;
struct FGameplayTag;
class ATagCharacter;
/**
 * 
 */
UCLASS()
class TAG_API UTagDisabledIndicator : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetupDelegate(APawn* OldPawn, APawn* NewPawn);
	void OnTagDisabled(const FGameplayTag CallbackTag, int32 NewCount);

private:
	UPROPERTY()
	ATagCharacter* TagCharacter;
	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComponent;
};
