// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TagIndicator.generated.h"

struct FGameplayTag;
class UAbilitySystemComponent;
struct FAIStimulus;
class ATagCharacter;
class UAIPerceptionComponent;
/**
 * 
 */
UCLASS()
class TAG_API UTagIndicator : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void SetupDelegate(APawn* OldPawn, APawn* NewPawn);
	UFUNCTION()
	void UpdateTagIndicator(const FGameplayTag Tag, int32 TagCount);

private:
	UPROPERTY()
	ATagCharacter* TagCharacter;
	UPROPERTY()
	UAbilitySystemComponent* TagCharacterAbilitySystemComponent;
	FDelegateHandle CouldTagSomeoneHandle;

	
};
