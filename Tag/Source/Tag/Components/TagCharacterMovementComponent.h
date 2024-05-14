// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TagCharacterMovementComponent.generated.h"

class ATagCharacter;

UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_None			UMETA(Hidden),
	CMOVE_Slide			UMETA(DisplayName = "Slide"),
	CMOVE_MAX			UMETA(Hidden),
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TAG_API UTagCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	class FSavedMove_Tag : public FSavedMove_Character
	{
		typedef FSavedMove_Character Super;

		//Flags
		uint8 Saved_bWantsToSprint : 1;

		uint8 Saved_bPrevWantsToCrouch : 1;

	public:
		FSavedMove_Tag();

		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(ACharacter* C) override;
	};

	class FNetworkPredictionData_Client_Tag : public FNetworkPredictionData_Client_Character
	{
	public:
		FNetworkPredictionData_Client_Tag(const UCharacterMovementComponent& ClientMovement);

		typedef FNetworkPredictionData_Client_Character Super;

		virtual FSavedMovePtr AllocateNewMove() override;
	};
	
	UPROPERTY(Transient)
	ATagCharacter* TagCharacter;
	
	uint8 bWantsToSprint : 1;
	uint8 bPrevWantsToCrouch : 1;
	
public:
	// Sets default values for this component's properties
	UTagCharacterMovementComponent();

	// Sprint
	UFUNCTION(BlueprintCallable, Category = "Sprint")
	void StartSprinting();
	UFUNCTION(BlueprintCallable, Category = "Sprint")
	void StopSprinting();

	// Crouch
	UFUNCTION(BlueprintCallable, Category = "Crouch")
	void StartCrouching();
	UFUNCTION(BlueprintCallable, Category = "Crouch")
	void StopCrouching();

	virtual bool IsMovingOnGround() const override;
	virtual bool CanCrouchInCurrentState() const override;

	void EnterSlide();
	void ExitSlide();
	
private:
	void PhysSlide(float deltaTime, int32 Iterations);
	bool GetSlideSurface(FHitResult& Hit) const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	float SprintSpeedMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crouch")
	float CrouchSpeedMultiplier;
	
	// Slide
	UPROPERTY(EditDefaultsOnly)
	float MinSlideSpeed=1050.f;
	UPROPERTY(EditDefaultsOnly)
	float MaxSlideSpeed=1800.f;
	UPROPERTY(EditDefaultsOnly)
	float SlideEnterImpulse=1000.f;
	UPROPERTY(EditDefaultsOnly)
	float SlideGravityForce=4000.f;
	UPROPERTY(EditDefaultsOnly)
	float SlideFrictionFactor=0.4f;
	UPROPERTY(EditDefaultsOnly)
	float BrakingDecelerationSliding=1000.f;

	UPROPERTY(EditDefaultsOnly)
	float GravityMultiplier=1.f;

	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
	virtual void InitializeComponent() override;

	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

	virtual bool CanAttemptJump() const override;
public:
	virtual float GetMaxSpeed() const override;
	virtual class FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	bool CanSlide() const;
	UFUNCTION(BlueprintPure)
	bool IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const;
	
};
