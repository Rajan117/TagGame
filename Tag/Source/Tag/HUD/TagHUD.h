// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TagHUD.generated.h"

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

	UPROPERTY()
	UCharacterOverlay* CharacterOverlay;
	
	void AddCharacterOverlay();

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> CharacterOverlayClass;

	virtual void BeginPlay() override;
};
