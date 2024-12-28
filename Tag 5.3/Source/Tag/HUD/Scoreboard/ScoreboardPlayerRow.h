// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Tag/PlayerState/TagPlayerState.h"
#include "ScoreboardPlayerRow.generated.h"

class ATagPlayerState;
class UScoreboard;
class UTextBlock;

/**
 * 
 */
UCLASS()
class TAG_API UScoreboardPlayerRow : public UUserWidget
{
	GENERATED_BODY()
public:
	void SpawnInitialize(ATagPlayerState* State, UScoreboard* ScoreboardRef);

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerNameText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ScoreText;

protected:
	UPROPERTY(BlueprintReadOnly)
	ATagPlayerState* PlayerState;
	
private:
	UPROPERTY()
	UScoreboard* Scoreboard;

	UFUNCTION()
	void ScoreUpdated(float NewScore);
	
public:
	float GetScore() const;
};
