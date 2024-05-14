// Fill out your copyright notice in the Description page of Project Settings.


#include "TagCharacterMovementComponent.h"

#include <string>

#include "Kismet/KismetSystemLibrary.h"
#include "Tag/Character/TagCharacter.h"

#pragma 


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

	return Super::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void UTagCharacterMovementComponent::FSavedMove_Tag::Clear()
{
	Super::Clear();

	Saved_bWantsToSprint = false;
	Saved_bPrevWantsToCrouch = false;
	Saved_bWantsToDash = false;
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
	SetMovementMode(MOVE_Custom, CMOVE_Slide);

	FindFloor(UpdatedComponent->GetComponentLocation(), CurrentFloor, true, nullptr);
}

void UTagCharacterMovementComponent::ExitSlide()
{
	bWantsToCrouch = false;
	const FQuat NewRotation = FRotationMatrix::MakeFromXZ(UpdatedComponent->GetForwardVector().GetSafeNormal2D(), FVector::UpVector).ToQuat();
	FHitResult Hit;
	SafeMoveUpdatedComponent(FVector::ZeroVector, NewRotation, true, Hit);
	SetMovementMode(MOVE_Walking);
}



void UTagCharacterMovementComponent::PhysSlide(float deltaTime, int32 Iterations)
{
	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}
	RestorePreAdditiveRootMotionVelocity();

	FHitResult SurfaceHit;
	if (!GetSlideSurface(SurfaceHit))
	{
		UE_LOG(LogTemp, Warning, TEXT("No slide surface"));
		ExitSlide();
		StartNewPhysics(deltaTime, Iterations);
		return;
	}

	if (Velocity.SizeSquared() < pow(MinSlideSpeed, 2))
	{
		UE_LOG(LogTemp, Warning, TEXT("Too slow"));
		ExitSlide();
		StartNewPhysics(deltaTime, Iterations);
		return;
	}

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
	
	if (!CanSlide())
	{
		ExitSlide();
	}

	//Update Outgoing Velcoity and Acceleration
	if (!bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / deltaTime;
	}
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

#pragma region Dash

void UTagCharacterMovementComponent::StartDash()
{
	bWantsToDash = true;
}

void UTagCharacterMovementComponent::StopDash()
{
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

float UTagCharacterMovementComponent::GetMaxSpeed() const
{
	const ATagCharacter* Owner = Cast<ATagCharacter>(GetOwner());
	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() No Owner"), *FString(__FUNCTION__));
		return Super::GetMaxSpeed();
	}

	if (bWantsToSprint)
	{
		return Owner->GetMoveSpeed() * SprintSpeedMultiplier;
	}

	if (bWantsToCrouch)
	{
		return Owner->GetMoveSpeed() * CrouchSpeedMultiplier;
	}
	
	return Owner->GetMoveSpeed();
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
			//SetMovementMode(MOVE_Custom, CMOVE_Slide);
		}
	}
	if (IsCustomMovementMode(CMOVE_Slide) && !bWantsToCrouch)
	{
		//SetMovementMode(MOVE_Walking);
	}
	if (bWantsToDash && CanDash())
	{
		PerformDash();
	}
	
	Super::UpdateCharacterStateBeforeMovement(DeltaSeconds);
}

void UTagCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);

	switch (CustomMovementMode)
	{
	case CMOVE_Slide:
		PhysSlide(deltaTime, Iterations);
		break;
	default:
		UE_LOG(LogTemp, Fatal, TEXT("Invalid Movement Mode"))
	}
}

bool UTagCharacterMovementComponent::CanAttemptJump() const
{
	return IsJumpAllowed() && (IsMovingOnGround() || IsFalling());
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
