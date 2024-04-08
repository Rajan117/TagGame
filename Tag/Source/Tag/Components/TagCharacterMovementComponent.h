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

		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(ACharacter* C) override;
	};

	class FNetworkPredictionData_Client_Tag : public FNetworkPredictionData_Client_Character
	{
	public:
		FNetworkPredictionData_Client_Tag(const UCharacterMovementComponent& ClientMovment);

		typedef FNetworkPredictionData_Client_Character Super;

		virtual FSavedMovePtr AllocateNewMove() override;
	};
	
	UPROPERTY(Transient)
	ATagCharacter* TagCharacter;
	
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

private:
	void EnterSlide();
	void ExitSlide();
	void PhysSlide(float deltaTime, int32 Iterations);
	bool GetSlideSurface(FHitResult& Hit) const;
	bool CanSlide() const;

protected:
	uint8 bWantsToSprint : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	float SprintSpeedMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crouch")
	float CrouchSpeedMultiplier;

	
	// Slide
	UPROPERTY(EditDefaultsOnly)
	float MinSlideSpeed=400.f;
	UPROPERTY(EditDefaultsOnly)
	float MaxSlideSpeed=600.f;
	UPROPERTY(EditDefaultsOnly)
	float SlideEnterImpulse=600.f;
	UPROPERTY(EditDefaultsOnly)
	float SlideGravityForce=4000.f;
	UPROPERTY(EditDefaultsOnly)
	float SlideFrictionFactor=.06f;
	UPROPERTY(EditDefaultsOnly)
	float BrakingDecelerationSliding=1000.f;

	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual void InitializeComponent() override;

public:
	virtual float GetMaxSpeed() const override;
	virtual class FNetworkPredictionData_Client* GetPredictionData_Client() const override;

	UFUNCTION(BlueprintPure)
	bool IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const;
	
};
