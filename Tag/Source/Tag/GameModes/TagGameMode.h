// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TagGameMode.generated.h"

class ATagPlayerController;
class UGameStartTimer;

/**
 * 
 */
UCLASS()
class TAG_API ATagGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ATagGameMode();
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly)
	float WarmupTime = 5.f;

	float LevelStartingTime = 0.f;

protected:
	virtual void BeginPlay() override;
	virtual void StartPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void OnMatchStateSet() override;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<class UGameplayEffect> TagEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UGameStartTimer> GameStartTimerClass;

	virtual void StartGameStartCountdown();
	virtual void ChooseTagger();
	virtual void StartGame();
	
private:
	float WarmupCountdownTime = 0.f;
	bool bTaggerChosen = false;
	TArray<ATagPlayerController*> Players;
	FTimerHandle ChooseTaggerHandle;
};
