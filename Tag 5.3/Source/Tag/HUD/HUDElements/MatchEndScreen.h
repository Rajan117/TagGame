// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MatchEndScreen.generated.h"

class UButton;
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
	UButton* RestartButton;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CountdownText;
	void StartTimer(float Time);
	
protected:
	UFUNCTION()
	virtual void CountdownTick();
	UFUNCTION()
	virtual void RestartButtonClicked();
	
private:
	FTimerHandle CountdownTimerHandle;
	int CountdownTime = 5;
};
