// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MatchEndScreen.generated.h"

class ATagPlayerController;
class ATagGameState;
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
	
protected:
	UFUNCTION()
	void OnMatchStateChanged(FName NewState);
	UFUNCTION()
	void SetupDelegate(APawn* OldPawn, APawn* NewPawn);
	void StartTimer(float Time);
	virtual void CountdownTick();
	
private:
	UPROPERTY()
	ATagGameState* TagGameState;
	UPROPERTY()
	ATagPlayerController* TagPlayerController;
	FTimerHandle CountdownTimerHandle;
	int CountdownTime = 5;
	void HandlePostMatch();
};
