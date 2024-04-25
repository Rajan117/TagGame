// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Scoreboard.generated.h"

class ATagPlayerState;
class UScoreboardPlayerRow;
class UVerticalBox;

/**
 * 
 */
UCLASS()
class TAG_API UScoreboard : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* PlayerBox;

	void AddPlayer(ATagPlayerState* State);

	void Setup();

	void SortPlayers();
	
protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UScoreboardPlayerRow> RowClass;
	
private:
	UPROPERTY()
	TArray<UScoreboardPlayerRow*> Rows;
};
