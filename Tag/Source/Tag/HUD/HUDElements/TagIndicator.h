// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TagIndicator.generated.h"

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

	UPROPERTY(VisibleDefaultsOnly)
	UAIPerceptionComponent* PerceptionComponent;

	UPROPERTY()
	ATagCharacter* TagCharacter;

	UFUNCTION()
	void UpdateTagIndicator(AActor* Actor, FAIStimulus Stimulus);
};
