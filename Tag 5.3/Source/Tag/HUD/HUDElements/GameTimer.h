// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameTimer.generated.h"

class ATagPlayerController;
class ATagGameState;
class UTextBlock;

/**
 * 
 */
UCLASS()
class TAG_API UGameTimer : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimerText;

protected:
	UFUNCTION()
	void OnMatchStateChanged(FName NewState);
	UFUNCTION()
	void SetupDelegate(APawn* OldPawn, APawn* NewPawn);
	void SetHUDTime() const;
	
private:
	void SetHUDTimerText(const float Time) const;
	UPROPERTY()
	ATagGameState* TagGameState;
	UPROPERTY()
	ATagPlayerController* TagPlayerController;
};
