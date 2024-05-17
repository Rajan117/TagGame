// Fill out your copyright notice in the Description page of Project Settings.


#include "TagCharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include <string>

#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Tag/Character/TagCharacter.h"

#pragma 

// Helper Macros
#if 1
float MacroDuration = 2.f;
#define SLOG(x) GEngine->AddOnScreenDebugMessage(-1, MacroDuration ? MacroDuration : -1.f, FColor::Yellow, x);
#define POINT(x, c) DrawDebugPoint(GetWorld(), x, 10, c, !MacroDuration, MacroDuration);
#define LINE(x1, x2, c) DrawDebugLine(GetWorld(), x1, x2, c, !MacroDuration, MacroDuration);
#define CAPSULE(x, c) DrawDebugCapsule(GetWorld(), x, CapHH(), CapR(), FQuat::Identity, c, !MacroDuration, MacroDuration);
#else
#define SLOG(x)
#define POINT(x, c)
#define LINE(x1, x2, c)
#define CAPSULE(x, c)
#endif

#pragma region Tag Saved Move

UTagCharacterMovementComponent::FSavedMove_Tag::FSavedMove_Tag()
{
	Saved_bWantsToSprint = 0;
	Saved_bPrevWantsToCrouch = 0;
	Saved_bWantsToDash = 0;
}

bool UTagCharacterMovementComponent::FSavedMove_Tag::CanCombineWith(const FSavedMovePtr& NewMove,
                                                                    ACharacter* InCharacter, float MaxDelta) const
{
	//Set which moves can be combined together. This will depend on the bit flags that are used.
	if (Saved_bWantsToSprint != ((FSavedMove_Tag*)&NewMove)->Saved_bWantsToSprint)
	{
		return false;
	}

	if (Saved_bWantsToDash != ((FSavedMove_Tag*)&NewMove)->Saved_bWantsToDash)
	{
		return false;
	}

	if (Saved_bWallRunIsRight != ((FSavedMove_Tag*)&NewMove)->Saved_bWallRunIsRight)
	{
		return false;
	}

	return Super::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void UTagCharacterMovementComponent::FSavedMove_Tag::Clear()
{
	Super::Clear();

	Saved_bWantsToSprint = false;
	Saved_bPrevWantsToCrouch = false;
	Saved_bWantsToDash = false;
	Saved_bWallRunIsRight = false;

	Saved_bTagPressedJump = false;
	Saved_bHadAnimRootMotion = false;
	Saved_bTransitionFinished = false;
}

uint8 UTagCharacterMovementComponent::FSavedMove_Tag::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (Saved_bWantsToSprint)
	{
		Result |= FLAG_Sprint;
	}

	if (Saved_bWantsToDash)
	{
		Result |= FLAG_Dash;
	}

	if (Saved_bTagPressedJump)
	{
		Result |= FLAG_JumpPressed;
	}

	return Result;
}

void UTagCharacterMovementComponent::FSavedMove_Tag::SetMoveFor(ACharacter* C, float InDeltaTime,
	FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	Super::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	if (const UTagCharacterMovementComponent* CharacterMovement = Cast<UTagCharacterMovementComponent>(C->GetCharacterMovement()))
	{
		Saved_bWantsToSprint = CharacterMovement->bWantsToSprint;
		Saved_bPrevWantsToCrouch = CharacterMovement->bPrevWantsToCrouch;
		Saved_bWantsToDash = CharacterMovement->bWantsToDash;
		Saved_bWallRunIsRight = CharacterMovement->bWallRunIsRight;

		Saved_bTagPressedJump = CharacterMovement->TagCharacter->bTagPressedJump;
		Saved_bHadAnimRootMotion = CharacterMovement->bHadAnimRootMotion;
		Saved_bTransitionFinished = CharacterMovement->bTransitionFinished;
	}
}

void UTagCharacterMovementComponent::FSavedMove_Tag::PrepMoveFor(ACharacter* C)
{
	Super::PrepMoveFor(C);

	if (UTagCharacterMovementComponent* CharacterMovement = Cast<UTagCharacterMovementComponent>(C->GetCharacterMovement()))
	{
		CharacterMovement->bWantsToSprint = Saved_bWantsToSprint;
		CharacterMovement->bPrevWantsToCrouch = Saved_bPrevWantsToCrouch;
		CharacterMovement->bWantsToDash = Saved_bWantsToDash;
		CharacterMovement->bWallRunIsRight = Saved_bWallRunIsRight;

		CharacterMovement->TagCharacter->bTagPressedJump = Saved_bTagPressedJump;
		CharacterMovement->bHadAnimRootMotion = Saved_bHadAnimRootMotion;
		CharacterMovement->bTransitionFinished = Saved_bTransitionFinished;
	}
}

#pragma endregion

#pragma region Rush Network Client Prediction Data

UTagCharacterMovementComponent::FNetworkPredictionData_Client_Tag::FNetworkPredictionData_Client_Tag(
	const UCharacterMovementComponent& ClientMovement) : Super(ClientMovement)
{
}

FSavedMovePtr UTagCharacterMovementComponent::FNetworkPredictionData_Client_Tag::AllocateNewMove()
{
	return MakeShared<FSavedMove_Tag>();
}

#pragma endregion 

#pragma region CMC

// Sets default values for this component's properties
UTagCharacterMovementComponent::UTagCharacterMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SprintSpeedMultiplier = 1.6f;
	CrouchSpeedMultiplier = 0.5f;

	bPrevWantsToCrouch = false;
	bWantsToSprint = true;
	NavAgentProps.bCanCrouch = true;
}

//Sprint
void UTagCharacterMovementComponent::StartSprinting()
{
	bWantsToSprint = true;
}

void UTagCharacterMovementComponent::StopSprinting()
{
	bWantsToSprint = false;
}

//Crouch
void UTagCharacterMovementComponent::StartCrouching()
{
	bWantsToCrouch = true;
}

void UTagCharacterMovementComponent::StopCrouching()
{
	bWantsToCrouch = false;
}

#pragma region Slide

//Sliding
void UTagCharacterMovementComponent::EnterSlide()
{
	bWantsToCrouch = true;
	bPrevWantsToCrouch = false;
	Velocity += Velocity.GetSafeNormal2D() * SlideEnterImpulse;
	//SetMovementMode(MOVE_Custom, CMOVE_Slide);

	FindFloor(UpdatedComponent->GetComponentLocation(), CurrentFloor, true, nullptr);
}

void UTagCharacterMovementComponent::ExitSlide()
{
	bWantsToCrouch = false;

	//SetMovementMode(MOVE_Walking);
}



void UTagCharacterMovementComponent::PhysSlide(float deltaTime, int32 Iterations)
{
	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}
	RestorePreAdditiveRootMotionVelocity();


	if (!CanSlide())
	{
		ExitSlide();
		return;
	}

	float remainingTime = deltaTime;

	// Perform the move
	while ( (remainingTime >= MIN_TICK_TIME) && (Iterations < MaxSimulationIterations) && CharacterOwner && (CharacterOwner->Controller || bRunPhysicsWithNoController || (CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy)) )
	{
		Iterations++;
		bJustTeleported = false;
		const float timeTick = GetSimulationTimeStep(remainingTime, Iterations);
		remainingTime -= timeTick;

		//Slide
		FHitResult SurfaceHit;
		//Surface Gravity
		Velocity += SlideGravityForce * GravityMultiplier * FVector::DownVector * deltaTime;

		Acceleration = FVector::ZeroVector;

		//Calculate velocity
		if (!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
		{
			CalcVelocity(deltaTime, SlideFrictionFactor, true, GetMaxBrakingDeceleration());
		}
		ApplyRootMotionToVelocity(deltaTime);

		//Perform Move
		Iterations++;
		bJustTeleported = false;

		FVector OldLocation = UpdatedComponent->GetComponentLocation();
		FHitResult Hit(1.f);
		FVector Adjusted = Velocity * deltaTime;
		FVector VelPlaneDir = FVector::VectorPlaneProject(Velocity, SurfaceHit.Normal).GetSafeNormal();
		FQuat NewRotation = FRotationMatrix::MakeFromXZ(VelPlaneDir, SurfaceHit.Normal).ToQuat();

		SafeMoveUpdatedComponent(Adjusted, NewRotation, true, Hit);

		if (Hit.Time < 1.f)
		{
			HandleImpact(Hit, deltaTime, Adjusted);
			SlideAlongSurface(Adjusted, (1.f - Hit.Time), Hit.Normal, Hit, true);
		}

		//Update Outgoing Velcoity and Acceleration
		if (!bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
		{
			Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / deltaTime;
		}

		// If we didn't move at all this iteration then abort (since future iterations will also be stuck).
		if (UpdatedComponent->GetComponentLocation() == OldLocation)
		{
			remainingTime = 0.f;
			break;
		}
	}
	FQuat OutgoingRotation = FRotationMatrix::MakeFromXZ(Velocity.GetSafeNormal2D(), FVector::UpVector).ToQuat();
	UKismetSystemLibrary::PrintString(this, OutgoingRotation.ToString());

	FHitResult OutgoingHit;
	SafeMoveUpdatedComponent(FVector::ZeroVector, OutgoingRotation, true, OutgoingHit);
}

bool UTagCharacterMovementComponent::GetSlideSurface(FHitResult& Hit) const
{
	const FVector Start = UpdatedComponent->GetComponentLocation();
	const FVector End = Start + CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.5f * FVector::DownVector;
	const FName ProfileName = TEXT("BlockAll");
	return GetWorld()->LineTraceTestByProfile(Start, End, ProfileName, TagCharacter->GetIgnoreCharacterParams());
}



bool UTagCharacterMovementComponent::CanSlide() const
{
	const FVector Start = UpdatedComponent->GetComponentLocation();
	const FVector End = Start + CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.5f * FVector::DownVector;
	const FName ProfileName = TEXT("BlockAll");
	const bool bValidSurface = GetWorld()->LineTraceTestByProfile(Start, End, ProfileName, TagCharacter->GetIgnoreCharacterParams());
	const bool bEnoughSpeed = Velocity.SizeSquared() > pow(MinSlideSpeed, 2);
	
	return bValidSurface && bEnoughSpeed;
}



#pragma endregion

#pragma region Wall Run

bool UTagCharacterMovementComponent::TryWallRun()
{
	if (!IsFalling()) return false;
	if (Velocity.SizeSquared2D() < pow(MinWallRunSpeed, 2)) return false;
	if (Velocity.Z < -MaxVerticalWallRunSpeed) return false;
	FVector Start = UpdatedComponent->GetComponentLocation();
	FVector LeftEnd = Start - UpdatedComponent->GetRightVector() * GetCapsuleRadius() * 2;
	FVector RightEnd = Start + UpdatedComponent->GetRightVector() * GetCapsuleRadius() * 2;
	auto Params = TagCharacter->GetIgnoreCharacterParams();
	FHitResult FloorHit, WallHit;
	// Check Player Height
	if (GetWorld()->LineTraceSingleByProfile(FloorHit, Start, Start + FVector::DownVector * (GetCapsuleHalfHeight() + MinWallRunHeight), "BlockAll", Params))
	{
		return false;
	}
	
	// Left Cast
	GetWorld()->LineTraceSingleByProfile(WallHit, Start, LeftEnd, "BlockAll", Params);
	if (WallHit.IsValidBlockingHit() && (Velocity | WallHit.Normal) < 0)
	{
		bWallRunIsRight = false;
	}
	// Right Cast
	else
	{
		GetWorld()->LineTraceSingleByProfile(WallHit, Start, RightEnd, "BlockAll", Params);
		if (WallHit.IsValidBlockingHit() && (Velocity | WallHit.Normal) < 0)
		{
			bWallRunIsRight = true;
		}
		else
		{
			return false;
		}
	}
	FVector ProjectedVelocity = FVector::VectorPlaneProject(Velocity, WallHit.Normal);
	if (ProjectedVelocity.SizeSquared2D() < pow(MinWallRunSpeed, 2)) return false;
	
	// Passed all conditions
	Velocity = ProjectedVelocity;
	Velocity.Z = FMath::Clamp(Velocity.Z, 0.f, MaxVerticalWallRunSpeed);
	SetMovementMode(MOVE_Custom, CMOVE_WallRun);
	return true;
}

void UTagCharacterMovementComponent::PhysWallRun(float deltaTime, int32 Iterations)
{
	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}
	if (!CharacterOwner || (!CharacterOwner->Controller && !bRunPhysicsWithNoController && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity() && (CharacterOwner->GetLocalRole() != ROLE_SimulatedProxy)))
	{
		Acceleration = FVector::ZeroVector;
		Velocity = FVector::ZeroVector;
		return;
	}
	
	bJustTeleported = false;
	float remainingTime = deltaTime;
	// Perform the move
	while ( (remainingTime >= MIN_TICK_TIME) && (Iterations < MaxSimulationIterations) && CharacterOwner && (CharacterOwner->Controller || bRunPhysicsWithNoController || (CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy)) )
	{
		Iterations++;
		bJustTeleported = false;
		const float timeTick = GetSimulationTimeStep(remainingTime, Iterations);
		remainingTime -= timeTick;
		const FVector OldLocation = UpdatedComponent->GetComponentLocation();
		
		FVector Start = UpdatedComponent->GetComponentLocation();
		FVector CastDelta = UpdatedComponent->GetRightVector() * GetCapsuleRadius() * 2;
		FVector End = bWallRunIsRight ? Start + CastDelta : Start - CastDelta;
		auto Params = TagCharacter->GetIgnoreCharacterParams();
		float SinPullAwayAngle = FMath::Sin(FMath::DegreesToRadians(WallRunPullAwayAngle));
		FHitResult WallHit;
		GetWorld()->LineTraceSingleByProfile(WallHit, Start, End, "BlockAll", Params);
		bool bWantsToPullAway = WallHit.IsValidBlockingHit() && !Acceleration.IsNearlyZero() && (Acceleration.GetSafeNormal() | WallHit.Normal) > SinPullAwayAngle;
		if (!WallHit.IsValidBlockingHit() || bWantsToPullAway)
		{
			SetMovementMode(MOVE_Falling);
			StartNewPhysics(remainingTime, Iterations);
			return;
		}
		// Clamp Acceleration
		Acceleration = FVector::VectorPlaneProject(Acceleration, WallHit.Normal);
		Acceleration.Z = 0.f;
		// Apply acceleration
		CalcVelocity(timeTick, 0.f, false, GetMaxBrakingDeceleration());
		Velocity = FVector::VectorPlaneProject(Velocity, WallHit.Normal);
		float TangentAccel = Acceleration.GetSafeNormal() | Velocity.GetSafeNormal2D();
		bool bVelUp = Velocity.Z > 0.f;
		Velocity.Z += GetGravityZ() * WallRunGravityScaleCurve->GetFloatValue(bVelUp ? 0.f : TangentAccel) * timeTick;
		if (Velocity.SizeSquared2D() < pow(MinWallRunSpeed, 2) || Velocity.Z < -MaxVerticalWallRunSpeed)
		{
			SetMovementMode(MOVE_Falling);
			StartNewPhysics(remainingTime, Iterations);
			return;
		}
		
		// Compute move parameters
		const FVector Delta = timeTick * Velocity; // dx = v * dt
		const bool bZeroDelta = Delta.IsNearlyZero();
		if ( bZeroDelta )
		{
			remainingTime = 0.f;
		}
		else
		{
			FHitResult Hit;
			SafeMoveUpdatedComponent(Delta, UpdatedComponent->GetComponentQuat(), true, Hit);
			FVector WallAttractionDelta = -WallHit.Normal * WallAttractionForce * timeTick;
			SafeMoveUpdatedComponent(WallAttractionDelta, UpdatedComponent->GetComponentQuat(), true, Hit);
		}
		if (UpdatedComponent->GetComponentLocation() == OldLocation)
		{
			remainingTime = 0.f;
			break;
		}
		Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / timeTick; // v = dx / dt
	}

	
	FVector Start = UpdatedComponent->GetComponentLocation();
	FVector CastDelta = UpdatedComponent->GetRightVector() * GetCapsuleRadius() * 2;
	FVector End = bWallRunIsRight ? Start + CastDelta : Start - CastDelta;
	auto Params = TagCharacter->GetIgnoreCharacterParams();
	FHitResult FloorHit, WallHit;
	GetWorld()->LineTraceSingleByProfile(WallHit, Start, End, "BlockAll", Params);
	GetWorld()->LineTraceSingleByProfile(FloorHit, Start, Start + FVector::DownVector * (GetCapsuleHalfHeight() + MinWallRunHeight * .5f), "BlockAll", Params);
	if (FloorHit.IsValidBlockingHit() || !WallHit.IsValidBlockingHit() || Velocity.SizeSquared2D() < pow(MinWallRunSpeed, 2))
	{
		SetMovementMode(MOVE_Falling);
	}
}



#pragma endregion 

#pragma region Dash

void UTagCharacterMovementComponent::StartDash()
{
	UKismetSystemLibrary::PrintString(this, "Start Dash");
	bWantsToDash = true;
}

void UTagCharacterMovementComponent::StopDash()
{
	UKismetSystemLibrary::PrintString(this, "Stop Dash");
	bWantsToDash = false;
}

bool UTagCharacterMovementComponent::CanDash() const
{
	return IsWalking() && !IsCrouching() || IsFalling();
}

void UTagCharacterMovementComponent::PerformDash()
{
	FVector DashDirection = (Acceleration.IsNearlyZero() ? UpdatedComponent->GetForwardVector() : Acceleration).GetSafeNormal2D();
	DashDirection += FVector::UpVector * 0.f;
	Velocity = DashImpulse * DashDirection;

	FQuat NewRotation = FRotationMatrix::MakeFromXZ(DashDirection, FVector::UpVector).ToQuat();
	FHitResult HitResult;
	SafeMoveUpdatedComponent(FVector::ZeroVector, NewRotation, false, HitResult);
	SetMovementMode(MOVE_Falling);
}

#pragma endregion

#pragma region Mantle

bool UTagCharacterMovementComponent::TryMantle()
{
	return false;
}

FVector UTagCharacterMovementComponent::GetMantleStartLocation(FHitResult FrontHit, FHitResult SurfaceHit,
	bool bTallMantle) const
{
	return FVector::ZeroVector;
}

void UTagCharacterMovementComponent::OnRep_ShortMantle()
{
}

void UTagCharacterMovementComponent::OnRep_TallMantle()
{
}

#pragma endregion 

float UTagCharacterMovementComponent::GetCapsuleRadius() const
{
	return CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius();
}

float UTagCharacterMovementComponent::GetCapsuleHalfHeight() const
{
	return CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
}

bool UTagCharacterMovementComponent::IsServer() const
{
	return TagCharacter->HasAuthority();
}


float UTagCharacterMovementComponent::GetMaxSpeed() const
{
	const ATagCharacter* Owner = Cast<ATagCharacter>(GetOwner());
	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() No Owner"), *FString(__FUNCTION__));
		return Owner->GetMoveSpeed();
	}
	
	if (MovementMode == MOVE_Walking && bWantsToSprint && !IsCrouching())
	{
		return Owner->GetMoveSpeed() * SprintSpeedMultiplier;
	}

	if (MovementMode == MOVE_Walking && bWantsToCrouch && IsCrouching())
	{
		return Owner->GetMoveSpeed() * CrouchSpeedMultiplier;
	}

	if (MovementMode != MOVE_Custom) return Owner->GetMoveSpeed();

	switch (CustomMovementMode)
	{
	case CMOVE_Slide:
		return MaxSlideSpeed;
	case CMOVE_WallRun:
		return MaxWallRunSpeed;
	default:
		UE_LOG(LogTemp, Fatal, TEXT("Invalid Movement Mode"));
		return -1.f;
	}
}

float UTagCharacterMovementComponent::GetMaxBrakingDeceleration() const
{
	if (MovementMode != MOVE_Custom) return Super::GetMaxBrakingDeceleration();

	switch (CustomMovementMode)
	{
	case CMOVE_Slide:
		return BrakingDecelerationSliding;
	case CMOVE_WallRun:
		return 0.f;
	default:
		UE_LOG(LogTemp, Fatal, TEXT("Invalid Movement Mode"))
		return -1.f;
	}
}

void UTagCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	//The Flags parameter contains the compressed input flags that are stored in the saved move.
	//UpdateFromCompressed flags simply copies the flags from the saved move into the movement component.
	//It basically just resets the movement component to the state when the move was made so it can simulate from there.
	bWantsToSprint = (Flags & FSavedMove_Tag::FLAG_Sprint) != 0;
	bWantsToDash = (Flags & FSavedMove_Tag::FLAG_Dash) != 0;
}

void UTagCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation,
	const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);
	
	bPrevWantsToCrouch = bWantsToCrouch;
}

void UTagCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

	if (PreviousMovementMode == MOVE_Custom && PreviousCustomMode == CMOVE_Slide) ExitSlide();
	if (IsCustomMovementMode(CMOVE_Slide)) EnterSlide();

	if (IsWallRunning() && GetOwnerRole() == ROLE_SimulatedProxy)
	{
		FVector Start = UpdatedComponent->GetComponentLocation();
		FVector End = Start + UpdatedComponent->GetRightVector() * GetCapsuleRadius() * 2;
		auto Params = TagCharacter->GetIgnoreCharacterParams();
		FHitResult WallHit;
		bWallRunIsRight = GetWorld()->LineTraceSingleByProfile(WallHit, Start, End, "BlockAll", Params);
	}
}

void UTagCharacterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	TagCharacter = Cast<ATagCharacter>(GetOwner());
}

void UTagCharacterMovementComponent::UpdateCharacterStateBeforeMovement(float DeltaSeconds)
{
	//UKismetSystemLibrary::PrintString(this, GetMovementName());
	
	// Slide
	if (MovementMode == MOVE_Walking && bWantsToCrouch)
	{
		if (CanSlide())
		{
			SetMovementMode(MOVE_Custom, CMOVE_Slide);
		}
	}
	if (IsCustomMovementMode(CMOVE_Slide) && !bWantsToCrouch)
	{
		SetMovementMode(MOVE_Walking);
	}
	//Dash
	if (bWantsToDash && CanDash())
	{
		//PerformDash();
	}
	//Wall Run
	if (IsFalling())
	{
		TryWallRun();
	}
	// Try Mantle
	if (TagCharacter->bTagPressedJump)
	{
		if (TryMantle())
		{
			TagCharacter->StopJumping();		
		}
		else
		{
			TagCharacter->bTagPressedJump = false;
			CharacterOwner->bPressedJump = true;
			CharacterOwner->CheckJumpInput(DeltaSeconds);
		}

	}
	
	// Transition
	if (bTransitionFinished)
	{
		UE_LOG(LogTemp, Warning, TEXT("FINISHED RM"))

		if (TransitionName == "Mantle")
		{
			if (IsValid(TransitionQueuedMontage))
			{
				SetMovementMode(MOVE_Flying);
				CharacterOwner->PlayAnimMontage(TransitionQueuedMontage, TransitionQueuedMontageSpeed);
				TransitionQueuedMontageSpeed = 0.f;
				TransitionQueuedMontage = nullptr;
			}
			else
			{
				SetMovementMode(MOVE_Walking);
			}
		}
		TransitionName = "";
		bTransitionFinished = false;
	}
	
	
	Super::UpdateCharacterStateBeforeMovement(DeltaSeconds);
}

void UTagCharacterMovementComponent::UpdateCharacterStateAfterMovement(float DeltaSeconds)
{
	Super::UpdateCharacterStateAfterMovement(DeltaSeconds);
}

void UTagCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);

	switch (CustomMovementMode)
	{
	case CMOVE_Slide:
		PhysSlide(deltaTime, Iterations);
		break;
	case CMOVE_WallRun:
		PhysWallRun(deltaTime, Iterations);
		break;
	default:
		UE_LOG(LogTemp, Fatal, TEXT("Invalid Movement Mode"))
	}
}

bool UTagCharacterMovementComponent::CanAttemptJump() const
{
	return IsJumpAllowed() && (IsMovingOnGround() || IsFalling() || IsWallRunning());
}

bool UTagCharacterMovementComponent::DoJump(bool bReplayingMoves)
{
	bool bWasWallRunning = IsWallRunning();
	if (Super::DoJump(bReplayingMoves))
	{
		if (bWasWallRunning)
		{
			FVector Start = UpdatedComponent->GetComponentLocation();
			FVector CastDelta = UpdatedComponent->GetRightVector() * GetCapsuleRadius() * 2;
			FVector End = bWallRunIsRight ? Start + CastDelta : Start - CastDelta;
			auto Params = TagCharacter->GetIgnoreCharacterParams();
			FHitResult WallHit;
			GetWorld()->LineTraceSingleByProfile(WallHit, Start, End, "BlockAll", Params);
			Velocity += WallHit.Normal * WallJumpOffForce;
		}
		return true;
	}
	return false;
}

void UTagCharacterMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UTagCharacterMovementComponent, Proxy_bShortMantle, COND_SkipOwner)
	DOREPLIFETIME_CONDITION(UTagCharacterMovementComponent, Proxy_bTallMantle, COND_SkipOwner)
}

bool UTagCharacterMovementComponent::IsMovingOnGround() const
{
	return Super::IsMovingOnGround() || IsCustomMovementMode(CMOVE_Slide);
}

bool UTagCharacterMovementComponent::CanCrouchInCurrentState() const
{
	return Super::CanCrouchInCurrentState() && IsMovingOnGround();
}

//Getters and Setters
FNetworkPredictionData_Client* UTagCharacterMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != NULL);

	if (!ClientPredictionData)
	{
		UTagCharacterMovementComponent* MutableThis = const_cast<UTagCharacterMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_Tag(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}

	return ClientPredictionData;
}

bool UTagCharacterMovementComponent::IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const
{
	return MovementMode == MOVE_Custom && CustomMovementMode == InCustomMovementMode;
}

#pragma endregion 
