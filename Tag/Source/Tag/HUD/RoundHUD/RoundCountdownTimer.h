// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RoundCountdownTimer.generated.h"

class ATagRoundBasedGameState;
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
	void OnRoundStarted(float RoundTime);
	UFUNCTION()
	void OnRoundEnded(float RoundIntervalTime);

private:
	float StartTime = 0;
	float TimePeriod = 0;
	UPROPERTY()
	ATagPlayerController* TagPlayerController;
	UPROPERTY()
	ATagRoundBasedGameState* TagRoundBasedGameState;
	void SetTimerText(const float Time) const;
};
