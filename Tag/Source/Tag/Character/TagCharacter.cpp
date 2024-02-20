// Fill out your copyright notice in the Description page of Project Settings.


#include "TagCharacter.h"

#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Net/UnrealNetwork.h"

ATagCharacter::ATagCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	check(FPSCameraComponent != nullptr);
	FPSCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));
	FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));
	FPSCameraComponent->bUsePawnControlRotation = true;

	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	check(FirstPersonMesh != nullptr);
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->SetupAttachment(FPSCameraComponent);
	FirstPersonMesh->bCastDynamicShadow = false;
	FirstPersonMesh->CastShadow = false;

	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->bCastDynamicShadow = true;
	GetMesh()->SetCastShadow(true);
	GetMesh()->SetCastHiddenShadow(true);

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	StandardAttributes = CreateDefaultSubobject<UStandardAttributeSet>(TEXT("StandardAttributeSet"));
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

void ATagCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		bTagged = true;
		SetupInitialAbilitiesAndEffects();
	}

	
}

void ATagCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();

	if (const APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(InputMappingContext, BaseMappingPriority);
		}
	}
}

void ATagCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATagCharacter, bTagged);
}

void ATagCharacter::SetupInitialAbilitiesAndEffects()
{
	if (!IsValid(AbilitySystemComponent) || !IsValid(StandardAttributes)) return;

	
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
	Server_Tag();
}

#pragma endregion

#pragma region Tagging

void ATagCharacter::Server_Tag_Implementation()
{
	if (!HasAuthority()) return;
	DetectTag();
	Multicast_Tag();
}

void ATagCharacter::Multicast_Tag_Implementation()
{
	PlayTagAnim();
}

void ATagCharacter::DetectTag() //Server Only
{
	if (!FPSCameraComponent || !GetWorld()) return;

	FHitResult TagHitResult;
	FVector Start = FPSCameraComponent->GetComponentLocation();
	Start.Z -= 10.0f; 
	FVector End = Start + GetViewRotation().Vector() * TagRange;
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	
	bool bHit = GetWorld()->SweepSingleByChannel(
		TagHitResult,
		Start,
		End,
		FQuat::Identity,
		ECollisionChannel::ECC_Pawn,
		FCollisionShape::MakeSphere(TagRadius), // Specify the radius of the sphere
		Params
	);

	if (bHit)
	{
		if (ATagCharacter* TaggedChar = Cast<ATagCharacter>(TagHitResult.GetActor()))
		{
			TagCharacter(TaggedChar);
		}
	}
	
	DrawDebugLine(
		GetWorld(),
		Start,
		End,
		bHit ? FColor::Green : FColor::Red,
		false,
		4.0f,
		0,
		2
	);
}

void ATagCharacter::TagCharacter(ATagCharacter* TaggedChar) //Server Only
{
	GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Yellow, TEXT("Tagged!"));
	if (TaggedChar && !TaggedChar->GetIsTagged())
	{
		bTagged = false;
		TaggedChar->SetTagged(true);
	}
}

void ATagCharacter::PlayTagAnim() const
{
	if (ThirdPersonTagAnimation)
	{
		if (UAnimInstance* TPAnimInstance = GetMesh()->GetAnimInstance())
		{
			TPAnimInstance->Montage_Play(ThirdPersonTagAnimation, 4.f);
		}
	}
	if (FirstPersonTagAnimation)
	{
		if (UAnimInstance* FPAnimInstance = FirstPersonMesh->GetAnimInstance())
		{
			FPAnimInstance->Montage_Play(FirstPersonTagAnimation, 4.f);
		}
	}
}

#pragma endregion
