// Fill out your copyright notice in the Description page of Project Settings.


#include "TagCharacter.h"

#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Tag/Components/TagCharacterMovementComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UserSettings/EnhancedInputUserSettings.h"

#include "Tag/GameplayAbilities/Abilities/AbilitySet.h"
#include "Tag/GameplayAbilities/Abilities/EIGameplayAbility.h"
#include "Tag/Controller/TagPlayerController.h"
#include "Tag/GameModes/TagGameMode.h"
#include "Tag/GameStates/TagGameState.h"
#include "Tag/PlayerState/TagPlayerState.h"

ATagCharacter::ATagCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UTagCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	check(FPSCameraComponent != nullptr);
	FPSCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));
	FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));
	FPSCameraComponent->bUsePawnControlRotation = true;

	SpeedLinesComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SpeedLinesComponent"));
	SpeedLinesComponent->SetupAttachment(RootComponent);

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
	AbilitySystemComponent->SetIsReplicated(true);
	StandardAttributes = CreateDefaultSubobject<UStandardAttributeSet>(TEXT("StandardAttributeSet"));

	TagCharacterMovementComponent = Cast<UTagCharacterMovementComponent>(GetCharacterMovement());

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));
	Sight->PeripheralVisionAngleDegrees = 60.f;
	Sight->SightRadius = TagSightRadius;
	Sight->LoseSightRadius = TagSightRadius;
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = true;

	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	PerceptionComponent->SetIsReplicated(true);
	PerceptionComponent->ConfigureSense(*Sight);
}

void ATagCharacter::ReportTag(ATagCharacter* TaggingCharacter, ATagCharacter* TaggedCharacter)
{
	if (ATagGameMode* TagGameMode = GetWorld()->GetAuthGameMode<ATagGameMode>())
	{
		TagGameMode->PlayerTagged(TaggingCharacter, TaggedCharacter);
	}
}

void ATagCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateScore(DeltaTime);
	ApplyWallRunTilt(DeltaTime);
	SetSprintFOV(DeltaTime);
	UpdateNiagaraVelocity();
}

void ATagCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	TagPlayerController = TagPlayerController == nullptr ? Cast<ATagPlayerController>(GetController()) : TagPlayerController;
	if (TagPlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(TagPlayerController->GetLocalPlayer()))
		{
			if (UEnhancedInputUserSettings* UserSettings =  Subsystem->GetUserSettings())
			{
				UserSettings->RegisterInputMappingContext(InputMappingContext);
			}
			FModifyContextOptions Opts = {};
			Opts.bNotifyUserSettings = true;
			Subsystem->AddMappingContext(InputMappingContext, 0, Opts);
		}
	}
	
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
		if (DashInputAction)
		{
			PlayerEnhancedInputComponent->BindAction(DashInputAction, ETriggerEvent::Started, this, &ATagCharacter::DashPressed);
			PlayerEnhancedInputComponent->BindAction(DashInputAction, ETriggerEvent::Completed, this, &ATagCharacter::DashReleased);
		}
	}
}

void ATagCharacter::BeginPlay()
{
	Super::BeginPlay();

	TagPlayerController = TagPlayerController == nullptr ? Cast<ATagPlayerController>(GetController()) : TagPlayerController;
	TagGameState = TagGameState == nullptr ? Cast<ATagGameState>(GetWorld()->GetGameState()) : TagGameState;

	BaseFOV = FPSCameraComponent->FieldOfView;
	
	SetupDelegates();
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
}

void ATagCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		AddCharacterAbilities();
		InitializeAttributes();
		AddStartupEffects();
	}
}

bool ATagCharacter::CanJumpInternal_Implementation() const
{
	return JumpIsAllowedInternal();
}

void ATagCharacter::UpdateScore(float DeltaTime)
{
	if (!HasAuthority() || !bShouldUpdateScore) return;
	if (GetIsTagged() && TagGameState->GetMatchState() != MatchState::PostMatch)
	{
		TimeTagged += DeltaTime;
		TagPlayerState = TagPlayerState == nullptr ? Cast<ATagPlayerState>(GetPlayerState()) : TagPlayerState;
		if (TagPlayerState)
		{
			TagPlayerState->ServerSetScore(TimeTagged);
		}
	}
}

void ATagCharacter::ApplyWallRunTilt(float DeltaTime)
{
	TagPlayerController = TagPlayerController == nullptr ? Cast<ATagPlayerController>(GetController()) : TagPlayerController;

	if (!TagCharacterMovementComponent || !TagPlayerController) return;

	float TargetRollAngle = 0.f;
	if (TagCharacterMovementComponent->IsWallRunning())
	{
		if (TagCharacterMovementComponent->WallRunningIsRight())
		{
			TargetRollAngle = -WallRunCameraRollAngle;
		}
		else
		{
			TargetRollAngle = WallRunCameraRollAngle;
		}
	}
	const float NormalizedTargetRollAngle = FRotator::NormalizeAxis(TargetRollAngle);
	
	FRotator ControlRotation = GetControlRotation();
	const float CurrentRollAngle = FRotator::NormalizeAxis(ControlRotation.Roll);
	
	const float NewRollAngle = FMath::FInterpTo(CurrentRollAngle, NormalizedTargetRollAngle, DeltaTime, WallRunCameraTiltInterpSpeed);
	ControlRotation.Roll = FRotator::NormalizeAxis(NewRollAngle);
	
	TagPlayerController->SetControlRotation(ControlRotation);
}

void ATagCharacter::UpdateNiagaraVelocity() const
{
	if (!SpeedLinesComponent) return;
	SpeedLinesComponent->SetVectorParameter(TEXT("User.CharacterVelocity"), GetVelocity()*-1.f);
}

void ATagCharacter::SetupDelegates()
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("State.Tagged")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ATagCharacter::OnTaggedStateChangedCallback);
	}

	if (PerceptionComponent)
	{
		PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ATagCharacter::CheckCouldTagSomeone);
	}
}

void ATagCharacter::CheckCouldTagSomeone(AActor* Actor, FAIStimulus Stimulus)
{
	if(!PerceptionComponent) return;
	TArray<AActor*> OutActors;
	PerceptionComponent->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), OutActors);

	for (AActor* PerceivedActor : OutActors)
	{
		const ATagCharacter* TagActor = Cast<ATagCharacter>(PerceivedActor);
		if (!TagActor) return;
		if (!TagActor->GetIsTagged())
		{
			OnCouldTagSomeoneChangedDelegate.Broadcast(true);
			Server_BroadcastCouldTagSomeone(true);
			return;
		}
		if (TagActor->GetIsTagged() && !GetIsTagged()) //For case where player is not tagged but if they were tagged by the tagged player they detect then they could tag someone.
		{
			OnCouldTagSomeoneChangedDelegate.Broadcast(true);
			Server_BroadcastCouldTagSomeone(true);
			return;
		}
	}
	OnCouldTagSomeoneChangedDelegate.Broadcast(false);
	Server_BroadcastCouldTagSomeone(false);
}

void ATagCharacter::Server_BroadcastCouldTagSomeone_Implementation(bool bCouldTagSomeone)
{
	Client_BroadcastCouldTagSomeone(bCouldTagSomeone);
}

void ATagCharacter::Client_BroadcastCouldTagSomeone_Implementation(bool bCouldTagSomeone)
{
	OnCouldTagSomeoneChangedDelegate.Broadcast(bCouldTagSomeone);
}

#pragma region Gameplay Ability System

void ATagCharacter::AddCharacterAbilities()
{
	if (!HasAuthority() || !IsValid(AbilitySystemComponent)) return;

	for (TSubclassOf<UEIGameplayAbility>& Ability : StartupAbilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability,
			0,
			static_cast<int32>(Ability.GetDefaultObject()->AbilityInputID),
			this));
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

	//Client and Server
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	if (const FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, 1, EffectContext); NewHandle.IsValid())
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
		}
	}
}

void ATagCharacter::SendLocalInputToGAS(const bool bPressed, const EAbilityInput AbilityID)
{
	if (!AbilitySystemComponent) return;
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, UEnum::GetValueAsString(AbilityID) + (bPressed ? FString(" Pressed") : FString(" Released")));

	if (bPressed)
	{
		AbilitySystemComponent->AbilityLocalInputPressed(static_cast<int32>(AbilityID));
	}
	else
	{
		AbilitySystemComponent->AbilityLocalInputReleased(static_cast<int32>(AbilityID));
	}
}

void ATagCharacter::OnTaggedStateChangedCallback(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (!AbilitySystemComponent) return;

	const FGameplayTag TagQuery = FGameplayTag::RequestGameplayTag(FName("State.Tagged"));
	OnTagStateChangedDelegate.Broadcast(NewCount>0);
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
	SendLocalInputToGAS(true, EAbilityInput::Slide);
}

void ATagCharacter::CrouchReleased()
{
	SendLocalInputToGAS(false, EAbilityInput::Crouch);
	SendLocalInputToGAS(false, EAbilityInput::Slide);
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

void ATagCharacter::DashPressed()
{
	//GetTagCharacterMovementComponent()->StartDash();
	SendLocalInputToGAS(true, EAbilityInput::Dash);
}

void ATagCharacter::DashReleased()
{
	//GetTagCharacterMovementComponent()->StopDash();
	SendLocalInputToGAS(false, EAbilityInput::Dash);
}

#pragma endregion

void ATagCharacter::Jump()
{
	Super::Jump();
	bTagPressedJump = true;
	bPressedJump = false;
}

void ATagCharacter::StopJumping()
{
	Super::StopJumping();
	bTagPressedJump = false;
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

bool ATagCharacter::GetIsTagged() const
{
	if (!AbilitySystemComponent) return false;
	const FGameplayTagContainer TagContainer = FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("Effect.Tagged")));
	return !AbilitySystemComponent->GetActiveEffectsWithAllTags(TagContainer).IsEmpty();
}

FCollisionQueryParams ATagCharacter::GetIgnoreCharacterParams() const
{
	FCollisionQueryParams Params;

	TArray<AActor*> CharacterChildren;
	GetAllChildActors(CharacterChildren);
	Params.AddIgnoredActors(CharacterChildren);
	Params.AddIgnoredActor(this);

	return Params;
}

void ATagCharacter::SetSprintFOV(float DeltaTime)
{
	if (IsLocallyControlled())
	{
		if (FOVCurve && FPSCameraComponent)
		{
			float Target = BaseFOV * FOVCurve->GetFloatValue(GetVelocity().Size());
			float NewFOV = FMath::FInterpTo(FPSCameraComponent->FieldOfView, Target, DeltaTime, 5.f);
			FPSCameraComponent->FieldOfView = NewFOV;
		}
	}
}