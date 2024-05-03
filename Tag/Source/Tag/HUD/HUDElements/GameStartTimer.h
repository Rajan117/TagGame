// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameStartTimer.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class TAG_API UGameStartTimer : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CountdownText;

	void StartTimer(float Time);

protected:
	virtual void CountdownTick();
	
private:
	FTimerHandle CountdownTimerHandle;
	int WarmupTime = 5;
};
