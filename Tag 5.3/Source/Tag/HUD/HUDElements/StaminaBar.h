// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StaminaBar.generated.h"

struct FOnAttributeChangeData;
class UStandardAttributeSet;
class UProgressBar;
class UAbilitySystemComponent;
class ATagCharacter;


/**
 * 
 */
UCLASS()
class TAG_API UStaminaBar : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaProgressBar;
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void SetupDelegate(APawn* OldPawn, APawn* NewPawn);
	virtual void StaminaChanged(const FOnAttributeChangeData& Data);

	UPROPERTY(EditDefaultsOnly)
	float StaminaBarInterpSpeed = 5.f;

private:
	UPROPERTY()
	ATagCharacter* TagCharacter;
	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY()
	UStandardAttributeSet* StandardAttributes;
	float TargetPercent;
	
};
