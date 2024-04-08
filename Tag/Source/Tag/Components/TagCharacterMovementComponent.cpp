// Fill out your copyright notice in the Description page of Project Settings.


#include "TagCharacterMovementComponent.h"

#include "Tag/Character/TagCharacter.h"

#pragma 


#pragma region Tag Saved Move

bool UTagCharacterMovementComponent::FSavedMove_Tag::CanCombineWith(const FSavedMovePtr& NewMove,
	ACharacter* InCharacter, float MaxDelta) const
{
	//Set which moves can be combined together. This will depend on the bit flags that are used.
	if (Saved_bWantsToSprint != ((FSavedMove_Tag*)&NewMove)->Saved_bWantsToSprint)
	{
		return false;
	}

	return Super::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void UTagCharacterMovementComponent::FSavedMove_Tag::Clear()
{
	Super::Clear();

	Saved_bWantsToSprint = false;
}

uint8 UTagCharacterMovementComponent::FSavedMove_Tag::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (Saved_bWantsToSprint)
	{
		Result |= FLAG_Custom_0;
	}

	return Result;
}

void UTagCharacterMovementComponent::FSavedMove_Tag::SetMoveFor(ACharacter* C, float InDeltaTime,
	FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	Super::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	UTagCharacterMovementComponent* CharacterMovement = Cast<UTagCharacterMovementComponent>(C->GetCharacterMovement());
	if (CharacterMovement)
	{
		Saved_bWantsToSprint = CharacterMovement->bWantsToSprint;
	}
}

void UTagCharacterMovementComponent::FSavedMove_Tag::PrepMoveFor(ACharacter* C)
{
	Super::PrepMoveFor(C);

	UTagCharacterMovementComponent* CharacterMovement = Cast<UTagCharacterMovementComponent>(C->GetCharacterMovement());
	if (CharacterMovement)
	{
	}
}

#pragma endregion

#pragma region Rush Network Client Prediction Data

UTagCharacterMovementComponent::FNetworkPredictionData_Client_Tag::FNetworkPredictionData_Client_Tag(
	const UCharacterMovementComponent& ClientMovment) : Super(ClientMovment)
{
}

FSavedMovePtr UTagCharacterMovementComponent::FNetworkPredictionData_Client_Tag::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_Tag());
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
	bWantsToCrouch = !bWantsToCrouch;
}

void UTagCharacterMovementComponent::StopCrouching()
{
	bWantsToCrouch = !bWantsToCrouch;
}

#pragma region Slide

//Sliding
void UTagCharacterMovementComponent::EnterSlide()
{
	bWantsToCrouch = true;
	Velocity += Velocity.GetSafeNormal2D() * SlideEnterImpulse;
	SetMovementMode(MOVE_Custom, CMOVE_Slide);

	FindFloor(UpdatedComponent->GetComponentLocation(), CurrentFloor, true, NULL);
}

void UTagCharacterMovementComponent::ExitSlide()
{
	bWantsToCrouch = false;
	SetMovementMode(MOVE_Walking);
}

void UTagCharacterMovementComponent::PhysSlide(float deltaTime, int32 Iterations)
{
	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}

	
	if (!CanSlide())
	{
		SetMovementMode(MOVE_Walking);
		StartNewPhysics(deltaTime, Iterations);
		return;
	}

	bJustTeleported = false;
	bool bCheckedFall = false;
	bool bTriedLedgeMove = false;
	float remainingTime = deltaTime;

	// Perform the move
	while ( (remainingTime >= MIN_TICK_TIME) && (Iterations < MaxSimulationIterations) && CharacterOwner && (CharacterOwner->Controller || bRunPhysicsWithNoController || (CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy)) )
	{
		Iterations++;
		bJustTeleported = false;
		const float timeTick = GetSimulationTimeStep(remainingTime, Iterations);
		remainingTime -= timeTick;

		// Save current values
		UPrimitiveComponent * const OldBase = GetMovementBase();
		const FVector PreviousBaseLocation = (OldBase != NULL) ? OldBase->GetComponentLocation() : FVector::ZeroVector;
		const FVector OldLocation = UpdatedComponent->GetComponentLocation();
		const FFindFloorResult OldFloor = CurrentFloor;

		// Ensure velocity is horizontal.
		MaintainHorizontalGroundVelocity();
		const FVector OldVelocity = Velocity;

		FVector SlopeForce = CurrentFloor.HitResult.Normal;
		SlopeForce.Z = 0.f;
		Velocity += SlopeForce * SlideGravityForce * deltaTime;
		
		Acceleration = Acceleration.ProjectOnTo(UpdatedComponent->GetRightVector().GetSafeNormal2D());

		// Apply acceleration
		CalcVelocity(timeTick, GroundFriction * SlideFrictionFactor, false, GetMaxBrakingDeceleration());
		
		// Compute move parameters
		const FVector MoveVelocity = Velocity;
		const FVector Delta = timeTick * MoveVelocity;
		const bool bZeroDelta = Delta.IsNearlyZero();
		FStepDownResult StepDownResult;
		bool bFloorWalkable = CurrentFloor.IsWalkableFloor();

		if ( bZeroDelta )
		{
			remainingTime = 0.f;
		}
		else
		{
			// try to move forward
			MoveAlongFloor(MoveVelocity, timeTick, &StepDownResult);

			if ( IsFalling() )
			{
				// pawn decided to jump up
				const float DesiredDist = Delta.Size();
				if (DesiredDist > KINDA_SMALL_NUMBER)
				{
					const float ActualDist = (UpdatedComponent->GetComponentLocation() - OldLocation).Size2D();
					remainingTime += timeTick * (1.f - FMath::Min(1.f,ActualDist/DesiredDist));
				}
				StartNewPhysics(remainingTime,Iterations);
				return;
			}
			else if ( IsSwimming() ) //just entered water
			{
				StartSwimming(OldLocation, OldVelocity, timeTick, remainingTime, Iterations);
				return;
			}
		}

		// Update floor.
		// StepUp might have already done it for us.
		if (StepDownResult.bComputedFloor)
		{
			CurrentFloor = StepDownResult.FloorResult;
		}
		else
		{
			FindFloor(UpdatedComponent->GetComponentLocation(), CurrentFloor, bZeroDelta, NULL);
		}


		// check for ledges here
		const bool bCheckLedges = !CanWalkOffLedges();
		if ( bCheckLedges && !CurrentFloor.IsWalkableFloor() )
		{
			// calculate possible alternate movement
			const FVector GravDir = FVector(0.f,0.f,-1.f);
			const FVector NewDelta = bTriedLedgeMove ? FVector::ZeroVector : GetLedgeMove(OldLocation, Delta, GravDir);
			if ( !NewDelta.IsZero() )
			{
				// first revert this move
				RevertMove(OldLocation, OldBase, PreviousBaseLocation, OldFloor, false);

				// avoid repeated ledge moves if the first one fails
				bTriedLedgeMove = true;

				// Try new movement direction
				Velocity = NewDelta / timeTick;
				remainingTime += timeTick;
				continue;
			}
			else
			{
				// see if it is OK to jump
				// @todo collision : only thing that can be problem is that oldbase has world collision on
				bool bMustJump = bZeroDelta || (OldBase == NULL || (!OldBase->IsQueryCollisionEnabled() && MovementBaseUtility::IsDynamicBase(OldBase)));
				if ( (bMustJump || !bCheckedFall) && CheckFall(OldFloor, CurrentFloor.HitResult, Delta, OldLocation, remainingTime, timeTick, Iterations, bMustJump) )
				{
					return;
				}
				bCheckedFall = true;

				// revert this move
				RevertMove(OldLocation, OldBase, PreviousBaseLocation, OldFloor, true);
				remainingTime = 0.f;
				break;
			}
		}
		else
		{
			// Validate the floor check
			if (CurrentFloor.IsWalkableFloor())
			{
				if (ShouldCatchAir(OldFloor, CurrentFloor))
				{
					HandleWalkingOffLedge(OldFloor.HitResult.ImpactNormal, OldFloor.HitResult.Normal, OldLocation, timeTick);
					if (IsMovingOnGround())
					{
						// If still walking, then fall. If not, assume the user set a different mode they want to keep.
						StartFalling(Iterations, remainingTime, timeTick, Delta, OldLocation);
					}
					return;
				}

				AdjustFloorHeight();
				SetBase(CurrentFloor.HitResult.Component.Get(), CurrentFloor.HitResult.BoneName);
			}
			else if (CurrentFloor.HitResult.bStartPenetrating && remainingTime <= 0.f)
			{
				// The floor check failed because it started in penetration
				// We do not want to try to move downward because the downward sweep failed, rather we'd like to try to pop out of the floor.
				FHitResult Hit(CurrentFloor.HitResult);
				Hit.TraceEnd = Hit.TraceStart + FVector(0.f, 0.f, MAX_FLOOR_DIST);
				const FVector RequestedAdjustment = GetPenetrationAdjustment(Hit);
				ResolvePenetration(RequestedAdjustment, Hit, UpdatedComponent->GetComponentQuat());
				bForceNextFloorCheck = true;
			}

			// check if just entered water
			if ( IsSwimming() )
			{
				StartSwimming(OldLocation, Velocity, timeTick, remainingTime, Iterations);
				return;
			}

			// See if we need to start falling.
			if (!CurrentFloor.IsWalkableFloor() && !CurrentFloor.HitResult.bStartPenetrating)
			{
				const bool bMustJump = bJustTeleported || bZeroDelta || (OldBase == NULL || (!OldBase->IsQueryCollisionEnabled() && MovementBaseUtility::IsDynamicBase(OldBase)));
				if ((bMustJump || !bCheckedFall) && CheckFall(OldFloor, CurrentFloor.HitResult, Delta, OldLocation, remainingTime, timeTick, Iterations, bMustJump) )
				{
					return;
				}
				bCheckedFall = true;
			}
		}
		
		// Allow overlap events and such to change physics state and velocity
		if (IsMovingOnGround() && bFloorWalkable)
		{
			// Make velocity reflect actual move
			if( !bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity() && timeTick >= MIN_TICK_TIME)
			{
				// TODO-RootMotionSource: Allow this to happen during partial override Velocity, but only set allowed axes?
				Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / timeTick;
				MaintainHorizontalGroundVelocity();
			}
		}

		// If we didn't move at all this iteration then abort (since future iterations will also be stuck).
		if (UpdatedComponent->GetComponentLocation() == OldLocation)
		{
			remainingTime = 0.f;
			break;
		}
	}


	FHitResult Hit;
	FQuat NewRotation = FRotationMatrix::MakeFromXZ(Velocity.GetSafeNormal2D(), FVector::UpVector).ToQuat();
	SafeMoveUpdatedComponent(FVector::ZeroVector, NewRotation, false, Hit);
}

bool UTagCharacterMovementComponent::GetSlideSurface(FHitResult& Hit) const
{
	FVector Start = UpdatedComponent->GetComponentLocation();
	FVector End = Start + CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.5f * FVector::DownVector;
	FName ProfileName = TEXT("BlockAll");
	return GetWorld()->LineTraceTestByProfile(Start, End, ProfileName, TagCharacter->GetIgnoreCharacterParams());
}

bool UTagCharacterMovementComponent::CanSlide() const
{
	FVector Start = UpdatedComponent->GetComponentLocation();
	FVector End = Start + CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.5f * FVector::DownVector;
	FName ProfileName = TEXT("BlockAll");
	bool bValidSurface = GetWorld()->LineTraceTestByProfile(Start, End, ProfileName, TagCharacter->GetIgnoreCharacterParams());
	bool bEnoughSpeed = Velocity.SizeSquared() > pow(MinSlideSpeed, 2);
	
	return bValidSurface && bEnoughSpeed;
}

#pragma endregion 

float UTagCharacterMovementComponent::GetMaxSpeed() const
{
	ATagCharacter* Owner = Cast<ATagCharacter>(GetOwner());
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
	bWantsToSprint = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
}

void UTagCharacterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	TagCharacter = Cast<ATagCharacter>(GetOwner());
}

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
