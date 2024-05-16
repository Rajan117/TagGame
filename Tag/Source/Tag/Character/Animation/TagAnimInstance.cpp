// Fill out your copyright notice in the Description page of Project Settings.


#include "TagAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Tag/Character/TagCharacter.h"
#include "Tag/Components/TagCharacterMovementComponent.h"

void UTagAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	TagCharacter = Cast<ATagCharacter>(TryGetPawnOwner());
}

void UTagAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	TagCharacter = TagCharacter == nullptr ? Cast<ATagCharacter>(TryGetPawnOwner()) : TagCharacter;
	if (!TagCharacter) return;

	CharVelocity = TagCharacter->GetVelocity();
	CharVelocity.Z = 0.f;
	CharSpeed = CharVelocity.Size();

	if (TagCharacter->GetTagCharacterMovementComponent())
	{
		bCharInAir = TagCharacter->GetTagCharacterMovementComponent()->IsFalling();
		bIsSliding = TagCharacter->GetTagCharacterMovementComponent()->IsSliding();
		bIsWallRunning = TagCharacter->GetTagCharacterMovementComponent()->IsWallRunning();
		bWallRunningIsRight = TagCharacter->GetTagCharacterMovementComponent()->WallRunningIsRight();
	}
}
