// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TagPlayerController.generated.h"

class ATagHUD;

/**
 * 
 */
UCLASS()
class TAG_API ATagPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	void SetCurrentEffectHUD();
protected:
	virtual void BeginPlay() override;
private:
	ATagHUD* TagHUD;
	
};
