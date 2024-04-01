// Fill out your copyright notice in the Description page of Project Settings.


#include "TagCharacterMovementComponent.h"

#include "Tag/Character/TagCharacter.h"

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

#pragma endregion 
