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
	CMOVE_WallRun			UMETA(DisplayName = "Wall Run"),
	CMOVE_MAX			UMETA(Hidden),
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TAG_API UTagCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	class FSavedMove_Tag : public FSavedMove_Character
	{
		typedef FSavedMove_Character Super;
	public:
		enum CompressedFlags
		{
			FLAG_Sprint			= 0x10,
			FLAG_Dash			= 0x20,
			FLAG_Custom_2		= 0x40,
			FLAG_Custom_3		= 0x80,
		};
		

		//Flags
		uint8 Saved_bWantsToSprint : 1;
		uint8 Saved_bWantsToDash : 1;
		uint8 Saved_bTagPressedJump : 1;
		
		uint8 Saved_bPrevWantsToCrouch : 1;
		uint8 Saved_bWallRunIsRight : 1;
		uint8 Saved_bHadAnimRootMotion:1;
		uint8 Saved_bTransitionFinished:1;

		
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

	bool bHadAnimRootMotion;
	bool bTransitionFinished;
	TSharedPtr<FRootMotionSource_MoveToForce> TransitionRMS;
	FString TransitionName;
	UPROPERTY(Transient) UAnimMontage* TransitionQueuedMontage;
	float TransitionQueuedMontageSpeed;
	int TransitionRMS_ID;
	
	uint8 bWantsToSprint : 1;
	uint8 bPrevWantsToCrouch : 1;
	uint8 bWantsToDash : 1;

	uint8 bWallRunIsRight : 1;
	
	float DashStartTime;

	UPROPERTY(ReplicatedUsing=OnRep_ShortMantle)
	bool Proxy_bShortMantle;
	UPROPERTY(ReplicatedUsing=OnRep_TallMantle)
	bool Proxy_bTallMantle;
	
public:
	UTagCharacterMovementComponent();

	//Sprint
	UFUNCTION(BlueprintCallable, Category = "Sprint")
	void StartSprinting();
	UFUNCTION(BlueprintCallable, Category = "Sprint")
	void StopSprinting();

	//Crouch
	UFUNCTION(BlueprintCallable, Category = "Crouch")
	void StartCrouching();
	UFUNCTION(BlueprintCallable, Category = "Crouch")
	void StopCrouching();
	virtual bool IsMovingOnGround() const override;
	virtual bool CanCrouchInCurrentState() const override;

	//Slide
	void EnterSlide();
	void ExitSlide();

	//Dash
	bool CanDash() const;
	void PerformDash();
	UFUNCTION(BlueprintCallable, Category = "Dash")
	void StartDash();
	UFUNCTION(BlueprintCallable, Category = "Dash")
	void StopDash();

	//WallRun
	virtual bool CanAttemptJump() const override;
	virtual bool DoJump(bool bReplayingMoves) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
private:
	//Slide
	void PhysSlide(float deltaTime, int32 Iterations);
	bool GetSlideSurface(FHitResult& Hit) const;

	//Wall Run
	bool TryWallRun();
	void PhysWallRun(float deltaTime, int32 Iterations);

	//Helpers
	float GetCapsuleRadius() const;
	float GetCapsuleHalfHeight() const;
	bool IsServer() const;

	//Mantle
	bool TryMantle();
	FVector GetMantleStartLocation(FHitResult FrontHit, FHitResult SurfaceHit, bool bTallMantle) const;
	UFUNCTION()
	void OnRep_ShortMantle();
	UFUNCTION()
	void OnRep_TallMantle();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	float SprintSpeedMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crouch")
	float CrouchSpeedMultiplier;
	
	//Slide
	UPROPERTY(EditDefaultsOnly)
	float MinSlideSpeed=850.f;
	UPROPERTY(EditDefaultsOnly)
	float MaxSlideSpeed=1000.f;
	UPROPERTY(EditDefaultsOnly)
	float SlideEnterImpulse=600.f;
	UPROPERTY(EditDefaultsOnly)
	float SlideGravityForce=6000.f;
	UPROPERTY(EditDefaultsOnly)
	float SlideFrictionFactor=0.4f;
	UPROPERTY(EditDefaultsOnly)
	float BrakingDecelerationSliding=1000.f;

	//Dash
	UPROPERTY(EditDefaultsOnly)
	float DashImpulse = 1000.f;
	UPROPERTY(EditDefaultsOnly)
	float DashCooldownDuration=1.f;
	UPROPERTY(EditDefaultsOnly)
	float AuthDashCooldownDuration=.9f;
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* DashMontage;

	//Wall Run
	UPROPERTY(EditDefaultsOnly)
	float MinWallRunSpeed=200.f;
	UPROPERTY(EditDefaultsOnly)
	float MaxWallRunSpeed=1000.f;
	UPROPERTY(EditDefaultsOnly)
	float MaxVerticalWallRunSpeed=400.f;
	UPROPERTY(EditDefaultsOnly)
	float WallRunPullAwayAngle=75;
	UPROPERTY(EditDefaultsOnly)
	float WallAttractionForce = 200.f;
	UPROPERTY(EditDefaultsOnly)
	float MinWallRunHeight=50.f;
	UPROPERTY(EditDefaultsOnly)
	UCurveFloat* WallRunGravityScaleCurve;
	UPROPERTY(EditDefaultsOnly)
	float WallJumpOffForce = 300.f;

	// Mantle
	UPROPERTY(EditDefaultsOnly, Category="Mantle")
	float MantleMaxDistance = 200;
	UPROPERTY(EditDefaultsOnly, Category="Mantle")
	float MantleReachHeight = 50;
	UPROPERTY(EditDefaultsOnly, Category="Mantle")
	float MinMantleDepth = 30;
	UPROPERTY(EditDefaultsOnly, Category="Mantle")
	float MantleMinWallSteepnessAngle = 75;
	UPROPERTY(EditDefaultsOnly, Category="Mantle")
	float MantleMaxSurfaceAngle = 40;
	UPROPERTY(EditDefaultsOnly, Category="Mantle")
	float MantleMaxAlignmentAngle = 45;
	UPROPERTY(EditDefaultsOnly, Category="Mantle")
	UAnimMontage* TallMantleMontage;
	UPROPERTY(EditDefaultsOnly, Category="Mantle")
	UAnimMontage* TransitionTallMantleMontage;
	UPROPERTY(EditDefaultsOnly, Category="Mantle")
	UAnimMontage* ProxyTallMantleMontage;
	UPROPERTY(EditDefaultsOnly, Category="Mantle")
	UAnimMontage* ShortMantleMontage;
	UPROPERTY(EditDefaultsOnly, Category="Mantle")
	UAnimMontage* TransitionShortMantleMontage;
	UPROPERTY(EditDefaultsOnly, Category="Mantle")
	UAnimMontage* ProxyShortMantleMontage;

	UPROPERTY(EditDefaultsOnly)
	float GravityMultiplier=1.f;

	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
	virtual void InitializeComponent() override;

	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;
	virtual void UpdateCharacterStateAfterMovement(float DeltaSeconds) override;
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
public:
	virtual float GetMaxSpeed() const override;
	virtual float GetMaxBrakingDeceleration() const override;
	virtual class FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	bool CanSlide() const;
	UFUNCTION(BlueprintPure)
	bool IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const;

	UFUNCTION(BlueprintPure) bool IsSliding() const { return IsCustomMovementMode(CMOVE_Slide); }
	UFUNCTION(BlueprintPure) bool IsWallRunning() const { return IsCustomMovementMode(CMOVE_WallRun); }
	UFUNCTION(BlueprintPure) bool WallRunningIsRight() const { return bWallRunIsRight; }
	UFUNCTION(BlueprintPure) bool IsMovementMode(EMovementMode InMovementMode) const;
};
