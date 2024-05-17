// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

#include "AbilitySystemInterface.h" 
#include "Tag/GameplayAbilities/Abilities/AbilityInput.h"
#include "Tag/GameplayAbilities/Attributes/StandardAttributeSet.h"
#include "TagCharacter.generated.h"


class UAbilitySet;
class UInputAction;
class UInputMappingContext;

class ATagPlayerController;
class ATagPlayerState;
class UTagCharacterMovementComponent;
struct FInputActionValue;

USTRUCT()
struct FAbilityInputToInputActionBinding
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	UInputAction* InputAction;
	UPROPERTY(EditDefaultsOnly)
	EAbilityInput AbilityInput;
};

USTRUCT()
struct FAbilityInputBindings
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TArray<FAbilityInputToInputActionBinding> Bindings;
};

UCLASS()
class TAG_API ATagCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ATagCharacter(const FObjectInitializer& ObjectInitializer);

	void ReportTag(ATagCharacter* TaggingCharacter, ATagCharacter* TaggedCharacter);
	
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FPSCameraComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* FirstPersonMesh;

	UPROPERTY(BlueprintReadOnly)
	float TimeTagged = 0.f;

	virtual void Tick(float DeltaTime) override;

	bool bShouldUpdateScore = true;

	bool bTagPressedJump;
	virtual void Jump() override;
	virtual void StopJumping() override;
	
protected:
	virtual void BeginPlay() override;
	virtual void PawnClientRestart() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PossessedBy(AController* NewController) override;

	virtual bool CanJumpInternal_Implementation() const override;

	virtual void UpdateScore(float DeltaTime);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement")
	UTagCharacterMovementComponent* TagCharacterMovementComponent;

private:
	UPROPERTY()
	ATagPlayerController* TagPlayerController;
	UPROPERTY()
	ATagPlayerState* TagPlayerState;
	
#pragma region Gameplay Ability System
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	virtual void AddCharacterAbilities();
	virtual void InitializeAttributes();
	virtual void AddStartupEffects();
	void SetupDelegates();
	void SendLocalInputToGAS(const bool bPressed, const EAbilityInput AbilityID);
	
	UPROPERTY(VisibleAnywhere, Category="Abilities")
	UAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY(VisibleAnywhere, Category="Abilities")
	UStandardAttributeSet* StandardAttributes;
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<class UEIGameplayAbility>> StartupAbilities;
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	//Effect Delegates
	void OnActiveGameplayEffectAddedCallback(UAbilitySystemComponent* Target, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle);
	
	//Attribute Delegates
	void OnMoveSpeedAttributeChanged(const FOnAttributeChangeData& MoveSpeedData);

	//Input Delegates
	UFUNCTION()
	void AbilityInputBindingPressedHandler(EAbilityInput AbilityInput);
	UFUNCTION()
	void AbilityInputBindingReleasedHandler(EAbilityInput AbilityInput);

	//Attribute Getters
	UFUNCTION(BlueprintCallable)
	float GetMoveSpeed() const;

#pragma endregion 

#pragma region Input
public:
	virtual void SetupPlayerInputComponent(
	class UInputComponent* PlayerInputComponent) override;
protected:
	// Input Mapping //
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enhanced Input | Mapping Contexts")
	UInputMappingContext* InputMappingContext;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enhanced Input | Mapping Contexts")
	int32 BaseMappingPriority = 0;

	// Input Actions //
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enhanced Input | Input Actions")
	UInputAction* MoveInputAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enhanced Input | Input Actions")
	UInputAction* LookInputAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enhanced Input | Input Actions")
	UInputAction* JumpInputAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enhanced Input | Input Actions")
	UInputAction* CrouchInputAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enhanced Input | Input Actions")
	UInputAction* TagInputAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enhanced Input | Input Actions")
	UInputAction* SprintInputAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enhanced Input | Input Actions")
	UInputAction* DashInputAction;


	// Input Functions //
	void EnhancedMove(const FInputActionValue& Value);
	void EnhancedLook(const FInputActionValue& Value);
	
	void JumpPressed();
	void JumpReleased();

	void CrouchPressed();
	void CrouchReleased();

	void TagPressed();
	void TagReleased();

	void SprintPressed();
	void SprintReleased();

	void DashPressed();
	void DashReleased();

#pragma endregion
	
protected:
	void PlayTagAnim() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tagging")
	float TagRange = 100;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tagging")
	float TagRadius = 10.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tagging | Animations")
	UAnimMontage* ThirdPersonTagAnimation;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tagging | Animations")
    UAnimMontage* FirstPersonTagAnimation;

public:
	UFUNCTION(BlueprintCallable)
	bool GetIsTagged();
	FORCEINLINE UTagCharacterMovementComponent* GetTagCharacterMovementComponent() const { return TagCharacterMovementComponent; }
	FCollisionQueryParams GetIgnoreCharacterParams() const;
};
