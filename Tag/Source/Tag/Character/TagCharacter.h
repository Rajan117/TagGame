// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

#include "TagCharacter.generated.h"

class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class TAG_API ATagCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATagCharacter();

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FPSCameraComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* FPSMesh;

protected:
	virtual void BeginPlay() override;
	virtual void PawnClientRestart() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma region Input
protected:
	// Input Mapping //
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enhanced Input | Mapping Contexts")
	UInputMappingContext* InputMappingContext;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enhanced Input | Mapping Contexts")
	int32 BaseMappingPriority = 0;

	// Input Actions //
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enhanced Input | Input Actions")
	UInputAction* MoveInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enhanced Input | Input Actions")
	UInputAction* LookInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enhanced Input | Input Actions")
	UInputAction* JumpInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enhanced Input | Input Actions")
	UInputAction* TagInputAction;

	// Input Functions //
	void EnhancedMove(const FInputActionValue& Value);
	void EnhancedLook(const FInputActionValue& Value);
	
	void JumpPressed();
	void JumpReleased();

	void TagPressed();

#pragma endregion 
	
};
