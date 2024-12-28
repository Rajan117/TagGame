// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Blueprint/UserWidget.h"
#include "TagCooldown.generated.h"

class UAbilitySystemComponent;
class ATagCharacter;
class UProgressBar;
/**
 * 
 */
UCLASS()
class TAG_API UTagCooldown : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* CooldownProgressBar;
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void SetupDelegate(APawn* OldPawn, APawn* NewPawn);

	void OnCooldownRemovedCallback(const FGameplayTag CallbackTag, int32 NewCount);
	float GetCoolDownPercentage() const;
	
private:
	UPROPERTY()
	ATagCharacter* TagCharacter;
	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComponent;
};
