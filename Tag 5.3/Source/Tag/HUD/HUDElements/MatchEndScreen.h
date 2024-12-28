// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MatchEndScreen.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class TAG_API UMatchEndScreen : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CountdownText;
	void StartTimer(float Time);
	
protected:
	UFUNCTION()
	virtual void CountdownTick();
	
private:
	FTimerHandle CountdownTimerHandle;
	int CountdownTime = 5;
};
