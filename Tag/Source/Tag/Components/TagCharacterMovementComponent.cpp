// Fill out your copyright notice in the Description page of Project Settings.


#include "TagCharacterMovementComponent.h"

#pragma region Tag Saved Move

bool UTagCharacterMovementComponent::FSavedMove_Tag::CanCombineWith(const FSavedMovePtr& NewMove,
	ACharacter* InCharacter, float MaxDelta) const
{
	return FSavedMove_Character::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void UTagCharacterMovementComponent::FSavedMove_Tag::Clear()
{
	FSavedMove_Character::Clear();
}

uint8 UTagCharacterMovementComponent::FSavedMove_Tag::GetCompressedFlags() const
{
	return FSavedMove_Character::GetCompressedFlags();
}

void UTagCharacterMovementComponent::FSavedMove_Tag::SetMoveFor(ACharacter* C, float InDeltaTime,
	FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);
}

void UTagCharacterMovementComponent::FSavedMove_Tag::PrepMoveFor(ACharacter* C)
{
	FSavedMove_Character::PrepMoveFor(C);
}

#pragma endregion

#pragma region Rush Network Client Prediction Data

UTagCharacterMovementComponent::FNetworkPredictionData_Client_Tag::FNetworkPredictionData_Client_Tag(
	const UCharacterMovementComponent& ClientMovment) : Super(ClientMovment)
{
}

FSavedMovePtr UTagCharacterMovementComponent::FNetworkPredictionData_Client_Tag::AllocateNewMove()
{
	return FNetworkPredictionData_Client_Character::AllocateNewMove();
}

#pragma endregion 

// Sets default values for this component's properties
UTagCharacterMovementComponent::UTagCharacterMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTagCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTagCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                   FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

