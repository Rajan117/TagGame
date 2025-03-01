// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TagGameMode.generated.h"

class ATagGameState;
class ATagPlayerState;
class ATagCharacter;
class ATagPlayerController;
class UGameStartTimer;
class UGameplayEffect;

namespace MatchState
{
	extern TAG_API const FName RoundStart; //During a round
	extern TAG_API const FName RoundEnd; //Round interval
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
	virtual void InitGameState() override;
	
	void PlayerTagged(
		ATagCharacter* TaggingCharacter,
		ATagCharacter* TaggedCharacter);

protected:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void OnMatchStateSet() override;

	//Match State
	virtual void HandleTick(float DeltaSeconds);
	void ChooseTagger();
	virtual void StartGame();
	virtual void StartRound();
	virtual void EndRound();

	//Tagging
	virtual void HandleTagEvent(
		ATagCharacter* TaggingCharacter,
		ATagCharacter* TaggedCharacter,
		ATagPlayerState* TaggingPlayer,
		ATagPlayerState* TaggedPlayer
	);
	void AnnounceTag(
		ATagPlayerState* TaggingPlayer,
		ATagPlayerState* TaggedPlayer) const;
	void RemoveTaggedEffect(const ATagCharacter* TagCharacter);
	bool TryTag(const ATagCharacter* CharacterToTag);
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<class UGameplayEffect> TagEffectClass;
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<class UGameplayEffect> SpeedBoostEffectClass;
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<class UGameplayEffect> TagDisabledEffectClass;

	//Timekeeping
	UPROPERTY(EditDefaultsOnly)
	float WarmupTime = 5.f;
	UPROPERTY(EditDefaultsOnly)
	float RoundTime = 30.f; 
	UPROPERTY(EditDefaultsOnly)
	int32 NumRounds = 1; //Set to 0 for infinite rounds
	UPROPERTY(EditDefaultsOnly)
	float RoundIntervalTime = 5.f;
	int32 CurrentRound = 0;
	UPROPERTY(EditDefaultsOnly)
	float RestartGameTime = 5;
	float LevelStartingTime = 0.f;
	float RoundStartingTime = 0.f;
	float LoadCountdownTime = 0.f;
	float LoadTime = 0.f;

	//Taggers
	UPROPERTY(EditDefaultsOnly)
	int32 MaxNumTaggers = 1;
	UPROPERTY(EditDefaultsOnly)
	int32 InitialNumTaggers = 1;
	UPROPERTY()
	TArray<ATagPlayerController*> TaggedPlayers;
	UPROPERTY()
	ATagGameState* TagGameState;

	//Eliminating Players
	virtual void EliminateTaggedPlayers();
	virtual void EliminatePlayer(ATagPlayerController* TagPlayerController);
	void AnnounceElimination(ATagPlayerState* EliminatedPLayer) const;
	void SwitchPlayerToSpectator(ATagPlayerController* TagPlayerController) const;
	
private:
	bool bTaggerChosen = false;
	FTimerHandle WarmupTimerHandle;
	FTimerHandle RestartGameTimerHandle;
	UPROPERTY()
	TArray<ATagPlayerController*> EliminatedPlayers;
};
