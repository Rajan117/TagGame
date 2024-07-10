// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tag/GameModes/TagGameMode.h"
#include "Tag/GameModes/TagRoundBasedGameMode.h"
#include "HotPotatoGameMode.generated.h"

UCLASS()
class TAG_API AHotPotatoGameMode : public ATagRoundBasedGameMode
{
	GENERATED_BODY()

public:
	AHotPotatoGameMode();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

protected:
	virtual void StartGame() override;
	virtual void StartRound() override;
	virtual void EndRound() override;
	virtual void EliminateTaggedPlayers();
	virtual void EliminatePlayer(ATagPlayerController* TagPlayerController);
	void AnnounceElimination(ATagPlayerState* EliminatedPLayer) const;

private:
	UPROPERTY()
	TArray<ATagPlayerController*> EliminatedPlayers;
};
