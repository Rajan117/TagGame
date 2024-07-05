// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RoundStatusNotification.generated.h"

class ATagRoundBasedGameState;
class UTextBlock;
class ATagPlayerController;
/**
 * 
 */
UCLASS()
class TAG_API URoundStatusNotification : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* StatusText;
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnRoundStarted(float RoundTime);
	UFUNCTION()
	void OnRoundEnded(float RoundIntervalTime);

	UFUNCTION()
	void ResetStatusText() const;

private:
	UPROPERTY()
	ATagRoundBasedGameState* TagRoundBasedGameState;
	FTimerHandle ResetStatusTextTimer;
	float StatusTextResetTime = 5.f;
};
