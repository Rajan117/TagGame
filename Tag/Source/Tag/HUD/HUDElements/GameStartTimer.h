// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameStartTimer.generated.h"

class ATagPlayerController;
class ATagGameState;
class UTextBlock;

/**
 * 
 */
UCLASS()
class TAG_API UGameStartTimer : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CountdownText;

	void StartTimer(float Time);
	void SetTime(float Time);

protected:
	virtual void CountdownTick();
	UFUNCTION()
	void OnMatchStateChanged(FName NewState);
	UFUNCTION()
	void SetupDelegate(APawn* OldPawn, APawn* NewPawn);
	
private:
	FTimerHandle CountdownTimerHandle;
	int WarmupTime = 5;

	UPROPERTY()
	ATagGameState* TagGameState;
	UPROPERTY()
	ATagPlayerController* TagPlayerController;
};
