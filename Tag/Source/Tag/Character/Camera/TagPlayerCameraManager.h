// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "TagPlayerCameraManager.generated.h"

UCLASS()
class TAG_API ATagPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATagPlayerCameraManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	float CrouchBlendDuration=0.2f;
	float CrouchBlendTime;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;
};
