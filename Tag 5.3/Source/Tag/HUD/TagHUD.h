// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TagHUD.generated.h"

class UMatchEndScreen;
class ATagGameState;
class ATagPlayerController;
class UCharacterOverlay;

/**
 * 
 */
UCLASS()
class TAG_API ATagHUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void DrawHUD() override;
	virtual void BeginPlay() override;

	UPROPERTY()
	UCharacterOverlay* CharacterOverlay;
	void AddCharacterOverlay();
	void RemoveCharacterOverlay() const;
	

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> CharacterOverlayClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMatchEndScreen> MatchEndWidgetClass;
	UFUNCTION()
	void SetupDelegate(APawn* OldPawn, APawn* NewPawn);
	UFUNCTION()
	void OnMatchStateChanged(FName NewState);

private:
	void HandlePostMatch();
	UPROPERTY()
	ATagPlayerController* TagPlayerController;
	UPROPERTY()
	ATagGameState* TagGameState;
	UPROPERTY()
	UMatchEndScreen* MatchEndWidgetRef;
};
