// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TaggedHUD.generated.h"

class ATagCharacter;
/**
 * 
 */
UCLASS()
class TAG_API UTaggedHUD : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetupDelegate(APawn* OldPawn, APawn* NewPawn);
	UFUNCTION()
	void OnTagStateChanged(bool bIsTagged);

private:
	UPROPERTY()
	ATagCharacter* TagCharacter;
};
