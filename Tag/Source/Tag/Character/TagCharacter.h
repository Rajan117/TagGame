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
	ATagCharacter();

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FPSCameraComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* FirstPersonMesh;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(
		class UInputComponent* PlayerInputComponent) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }

protected:
	virtual void BeginPlay() override;
	virtual void PawnClientRestart() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

#pragma region Gameplay Ability System
	
	void SetupInitialAbilitiesAndEffects();	
	
	UPROPERTY(VisibleAnywhere, Category="Abilities")
	UAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY(VisibleAnywhere, Category="Abilities")
	UStandardAttributeSet* StandardAttributes;
	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	TSubclassOf<UAbilitySet> InitialAbilitySet;
	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	TSubclassOf<UGameplayEffect> InitialGameplayEffect;
	UPROPERTY(Transient)
	TArray<FGameplayAbilitySpecHandle> InitiallyGrantedAbilitySpecHandles;
	UPROPERTY(EditDefaultsOnly)
	FAbilityInputBindings AbilityInputBindings;
	
	void OnMoveSpeedAttributeChanged(const FOnAttributeChangeData& MoveSpeedData);

	UFUNCTION()
	void AbilityInputBindingPressedHandler(EAbilityInput AbilityInput);
	UFUNCTION()
	void AbilityInputBindingReleasedHandler(EAbilityInput AbilityInput);

#pragma endregion 

#pragma region Input
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
	UInputAction* TagInputAction;

	// Input Functions //
	void EnhancedMove(const FInputActionValue& Value);
	void EnhancedLook(const FInputActionValue& Value);
	
	void JumpPressed();
	void JumpReleased();

	void TagPressed();

#pragma endregion
	
#pragma region Tagging

public:
	void Tag();
	
protected:
	UFUNCTION(Server, Reliable)
	void Server_Tag();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Tag();

	void DetectTag();
	void TagCharacter(ATagCharacter* TaggedChar);
	void PlayTagAnim() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tagging")
	float TagRange = 100;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tagging")
	float TagRadius = 10;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tagging | Animations")
	UAnimMontage* ThirdPersonTagAnimation;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tagging | Animations")
    UAnimMontage* FirstPersonTagAnimation;

private:
	UPROPERTY(Replicated)
	bool bTagged;

#pragma endregion 

public:
	FORCEINLINE void SetTagged(const bool bIsTagged) { bTagged = bIsTagged; }
	FORCEINLINE bool GetIsTagged() const { return bTagged; }
};
