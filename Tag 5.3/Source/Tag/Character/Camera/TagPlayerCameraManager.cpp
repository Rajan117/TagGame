// Fill out your copyright notice in the Description page of Project Settings.


#include "TagPlayerCameraManager.h"

#include "Tag/Character/TagCharacter.h"
#include "Tag/Components/TagCharacterMovementComponent.h"


// Sets default values
ATagPlayerCameraManager::ATagPlayerCameraManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATagPlayerCameraManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATagPlayerCameraManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATagPlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);

	if (ATagCharacter* TagCharacter = Cast<ATagCharacter>(GetOwningPlayerController()->GetPawn()))
	{
		if (UTagCharacterMovementComponent* TagCMC = TagCharacter->GetTagCharacterMovementComponent())
		{
			FVector TargetCrouchOffset = FVector(
				0,
				0,
				TagCMC->GetCrouchedHalfHeight() - TagCharacter->GetClass()->GetDefaultObject<ACharacter>()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()
			);
			
			FVector Offset = FMath::Lerp(FVector::ZeroVector, TargetCrouchOffset, FMath::Clamp(CrouchBlendTime / CrouchBlendDuration, 0.f, 1.f));

			if (TagCMC->IsCrouching())
			{
				CrouchBlendTime = FMath::Clamp(CrouchBlendTime + DeltaTime, 0.f, CrouchBlendDuration);
				Offset -= TargetCrouchOffset;
			}
			else
			{
				CrouchBlendTime = FMath::Clamp(CrouchBlendTime - DeltaTime, 0.f, CrouchBlendDuration);
			}

			OutVT.POV.Location += Offset;
		}
	}
}

