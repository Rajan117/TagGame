// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Score.generated.h"

class UTextBlock;
class ATagPlayerState;
/**
 * 
 */
UCLASS()
class TAG_API UScore : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreText;

protected:
	UFUNCTION()
	void OnScoreUpdated(const float Score);
private:
	UPROPERTY()
	ATagPlayerState* TagPlayerState;
	
};
