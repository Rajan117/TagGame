// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameModeText.generated.h"

class UTextBlock;
class ATagCharacter;
/**
 * 
 */
UCLASS()
class TAG_API UGameModeText : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void UpdateGameModeText(APawn* OldPawn, APawn* NewPawn);


private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* GameModeTextBlock;
	UPROPERTY()
	ATagCharacter* TagCharacter;
};
