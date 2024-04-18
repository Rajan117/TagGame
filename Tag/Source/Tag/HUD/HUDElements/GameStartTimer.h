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

	int WarmupTime = 5;

protected:
	virtual void NativeConstruct() override;
	virtual void CountdownTick();
	
private:
	FTimerHandle CountdownTimerHandle;
	
};
