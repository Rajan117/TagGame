// Fill out your copyright notice in the Description page of Project Settings.


#include "TagCharacter.h"

#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/TagAnimInstance.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Tag/Components/TagCharacterMovementComponent.h"

#include "Tag/GameplayAbilities/Abilities/AbilitySet.h"
#include "Tag/GameplayAbilities/Abilities/EIGameplayAbility.h"
#include "Tag/Controller/TagPlayerController.h"

ATagCharacter::ATagCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UTagCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
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

	TagCharacterMovementComponent = Cast<UTagCharacterMovementComponent>(GetCharacterMovement());
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

		if (CrouchInputAction)
		{
			PlayerEnhancedInputComponent->BindAction(CrouchInputAction, ETriggerEvent::Started, this, &ATagCharacter::CrouchPressed);
			PlayerEnhancedInputComponent->BindAction(CrouchInputAction, ETriggerEvent::Completed, this, &ATagCharacter::CrouchReleased);
		}
		
		if (TagInputAction)
		{
			PlayerEnhancedInputComponent->BindAction(TagInputAction, ETriggerEvent::Started, this, &ATagCharacter::TagPressed);
			PlayerEnhancedInputComponent->BindAction(TagInputAction, ETriggerEvent::Completed, this, &ATagCharacter::TagReleased);
		}

		if (SprintInputAction)
		{
			PlayerEnhancedInputComponent->BindAction(SprintInputAction, ETriggerEvent::Started, this, &ATagCharacter::SprintPressed);
			PlayerEnhancedInputComponent->BindAction(SprintInputAction, ETriggerEvent::Completed, this, &ATagCharacter::SprintReleased);
		}
	}
}

void ATagCharacter::BeginPlay()
{
	Super::BeginPlay();

	TagPlayerController = TagPlayerController == nullptr ? Cast<ATagPlayerController>(GetController()) : TagPlayerController;
	
	SetupDelegates();
	AddCharacterAbilities();
	InitializeAttributes();
	AddStartupEffects();
	
	if (HasAuthority() && AbilitySystemComponent)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(TagEffect, 0, EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
		}
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

#pragma region Gameplay Ability System

void ATagCharacter::AddCharacterAbilities()
{
	if (!HasAuthority() || !IsValid(AbilitySystemComponent)) return;

	for (TSubclassOf<UEIGameplayAbility>& Ability : StartupAbilities)
	{
		SendLocalInputToGAS(true, Ability.GetDefaultObject()->AbilityInputID);
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, 0, static_cast<int32>(Ability.GetDefaultObject()->AbilityInputID), this));
	}
	
}

void ATagCharacter::InitializeAttributes()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	if (!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	// Can run on Server and Client
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	const FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, 1, EffectContext);
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
	}
}

void ATagCharacter::AddStartupEffects()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 0, EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
			//UKismetSystemLibrary::PrintString(this, ActiveGEHandle.WasSuccessfullyApplied() ? TEXT("Effect Applied") : TEXT("Failed To Apply Effect"));
		}
	}
}

void ATagCharacter::SetupDelegates()
{
	if (!AbilitySystemComponent) return;
	
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &ATagCharacter::OnActiveGameplayEffectAddedCallback);
}

void ATagCharacter::SendLocalInputToGAS(const bool bPressed, const EAbilityInput AbilityID)
{
	if (!AbilitySystemComponent) return;
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, UEnum::GetValueAsString(AbilityID) + (bPressed ? FString(" Pressed") : FString(" Released")));

	if (bPressed)
	{
		AbilitySystemComponent->AbilityLocalInputPressed(static_cast<int32>(AbilityID));
	}
	else
	{
		AbilitySystemComponent->AbilityLocalInputReleased(static_cast<int32>(AbilityID));
	}
}

void ATagCharacter::OnActiveGameplayEffectAddedCallback(UAbilitySystemComponent* Target,
	const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle)
{
	TagPlayerController = TagPlayerController == nullptr ? Cast<ATagPlayerController>(GetController()) : TagPlayerController;
	if (TagPlayerController)
	{
		TagPlayerController->SetCurrentEffectHUD(SpecApplied.ToSimpleString());
	}
}

void ATagCharacter::OnMoveSpeedAttributeChanged(const FOnAttributeChangeData& MoveSpeedData)
{
}

void ATagCharacter::AbilityInputBindingPressedHandler(EAbilityInput AbilityInput)
{
	if (!IsValid(AbilitySystemComponent)) return;
	AbilitySystemComponent->AbilityLocalInputPressed(static_cast<uint32>(AbilityInput));
}

void ATagCharacter::AbilityInputBindingReleasedHandler(EAbilityInput AbilityInput)
{
	if (!IsValid(AbilitySystemComponent)) return;
	AbilitySystemComponent->AbilityLocalInputReleased(static_cast<uint32>(AbilityInput));
}

float ATagCharacter::GetMoveSpeed() const
{
	if (IsValid(StandardAttributes))
	{
		return StandardAttributes->GetMoveSpeed();
	}

	return 0.0f;
}

#pragma endregion 

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
	SendLocalInputToGAS(true, EAbilityInput::Jump);
}

void ATagCharacter::JumpReleased()
{
	SendLocalInputToGAS(false, EAbilityInput::Jump);
}

void ATagCharacter::CrouchPressed()
{
	SendLocalInputToGAS(true, EAbilityInput::Crouch);
}

void ATagCharacter::CrouchReleased()
{
	SendLocalInputToGAS(false, EAbilityInput::Crouch);
}

void ATagCharacter::TagPressed()
{
	SendLocalInputToGAS(true, EAbilityInput::Tag);
}

void ATagCharacter::TagReleased()
{
	SendLocalInputToGAS(false, EAbilityInput::Tag);
}

void ATagCharacter::SprintPressed()
{
	SendLocalInputToGAS(true, EAbilityInput::Sprint);
}

void ATagCharacter::SprintReleased()
{
	SendLocalInputToGAS(false, EAbilityInput::Sprint);
}

#pragma endregion

#pragma region Tagging

void ATagCharacter::Tag()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	if (TagEffect)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(TagEffect, 0, EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
			//UKismetSystemLibrary::PrintString(this, ActiveGEHandle.WasSuccessfullyApplied() ? TEXT("Tagger Chosen") : TEXT("Failed To Apply Tagged Effect"));
		}
	}
}

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
