// Fill out your copyright notice in the Description page of Project Settings.


#include "TagCharacter.h"

#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
 	

#include "EnhancedInputSubsystems.h"

ATagCharacter::ATagCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	check(FPSCameraComponent != nullptr);
	FPSCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));
	FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));
	FPSCameraComponent->bUsePawnControlRotation = true;

	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	check(FPSMesh != nullptr);
	FPSMesh->SetOnlyOwnerSee(true);
	FPSMesh->SetupAttachment(FPSCameraComponent);
	FPSMesh->bCastDynamicShadow = false;
	FPSMesh->CastShadow = false;
	GetMesh()->SetOwnerNoSee(true);
}

void ATagCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ATagCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(InputMappingContext, BaseMappingPriority);
		}
	}
}

void ATagCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATagCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveInputAction)
		{
			PlayerEnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ATagCharacter::EnhancedMove);
		}

		if (LookInputAction)
		{
			PlayerEnhancedInputComponent->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &ATagCharacter::EnhancedLook);
		}

		if (JumpInputAction)
		{
			PlayerEnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Started, this, &ATagCharacter::JumpPressed);
			PlayerEnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Completed, this, &ATagCharacter::JumpReleased);
		}

		if (TagInputAction)
		{
			PlayerEnhancedInputComponent->BindAction(TagInputAction, ETriggerEvent::Started, this, &ATagCharacter::TagPressed);
		}
	}
}

#pragma region Input

void ATagCharacter::EnhancedMove(const FInputActionValue& Value)
{
	if (Value.GetMagnitude() != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value[0]);
		AddMovementInput(GetActorRightVector(), Value[1]);
	}
}

void ATagCharacter::EnhancedLook(const FInputActionValue& Value)
{
	AddControllerPitchInput(Value[1] * -1.f);
	AddControllerYawInput(Value[0]);
}

void ATagCharacter::JumpPressed()
{
	Jump();
}

void ATagCharacter::JumpReleased()
{
	StopJumping();
}

void ATagCharacter::TagPressed()
{
	GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Yellow, TEXT("Tag!"));

}

#pragma endregion
