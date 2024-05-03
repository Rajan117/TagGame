// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TagGameModeBase.generated.h"

class ATagPlayerController;
class UGameStartTimer;

/**
 * 
 */
UCLASS()
class TAG_API ATagGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void StartPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<class UGameplayEffect> TagEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UGameStartTimer> GameStartTimerClass;

	virtual void StartGameStartCountdown();
	virtual void ChooseTagger();
	virtual void StartGame();

private:
	bool bTaggerChosen = false;
	TArray<ATagPlayerController*> Players;
	FTimerHandle ChooseTaggerHandle;
};
