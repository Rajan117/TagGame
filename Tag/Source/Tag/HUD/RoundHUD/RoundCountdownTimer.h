// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RoundCountdownTimer.generated.h"

class ATagPlayerController;
class UTextBlock;

/**
 * 
 */
UCLASS()
class TAG_API URoundCountdownTimer : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CountdownText;
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void SetupDelegate(APawn* OldPawn, APawn* NewPawn);

private:
	UPROPERTY()
	ATagPlayerController* TagPlayerController;
};
