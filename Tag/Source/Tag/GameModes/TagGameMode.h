// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TagGameMode.generated.h"

class ATagPlayerState;
class ATagCharacter;
class ATagPlayerController;
class UGameStartTimer;
class UGameplayEffect;

namespace MatchState
{
	extern TAG_API const FName Warmup; //Pre-game warmup period
	extern TAG_API const FName InMatch; //Actual game
	extern TAG_API const FName PostMatch; //After the game has ended
}

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
	UPROPERTY(EditDefaultsOnly)
	float MatchTime = 120.f;
	UPROPERTY(EditDefaultsOnly)
	float RestartGameTime = 5;

	float LevelStartingTime = 0.f;
	float RoundStartingTime = 0.f;

	void PlayerTagged(
		ATagCharacter* TaggingCharacter,
		ATagCharacter* TaggedCharacter);

protected:
	virtual void BeginPlay() override;
	virtual void StartPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void OnMatchStateSet() override;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<UGameplayEffect> TagEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UGameStartTimer> GameStartTimerClass;

	virtual void StartGameStartCountdown();
	virtual void ChooseTagger();
	virtual void StartGame();
	virtual void StartGameRestartCountdown();

	virtual void HandleTagEvent(
		ATagCharacter* TaggingCharacter,
		ATagCharacter* TaggedCharacter,
		ATagPlayerState* TaggingPlayer,
		ATagPlayerState* TaggedPlayer
	);
	void AnnounceTag(
		ATagPlayerState* TaggingPlayer,
		ATagPlayerState* TaggedPlayer);
	
private:
	TArray<ATagPlayerController*> Players;
	
	float LoadCountdownTime = 0.f;
	float LoadTime = 0.f;
	
	bool bTaggerChosen = false;
	FTimerHandle ChooseTaggerHandle;

	FTimerHandle RestartGameHandle;
	
};
