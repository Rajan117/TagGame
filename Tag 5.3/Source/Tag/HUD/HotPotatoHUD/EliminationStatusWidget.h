// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EliminationStatusWidget.generated.h"

class ATagGameState;
class ATagPlayerState;
class UTextBlock;
/**
 * 
 */
UCLASS()
class TAG_API UEliminationStatusWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* StatusText;

protected:
	UFUNCTION()
	void OnPlayerEliminated(ATagPlayerState* EliminatedPlayer);

private:
	UPROPERTY()
	ATagGameState* TagGameState;
};
