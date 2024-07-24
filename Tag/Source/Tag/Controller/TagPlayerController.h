// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Settings/EditorLoadingSavingSettings.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "TagPlayerController.generated.h"

class UPauseMenu;
class UScoreboard;
class ATagPlayerState;

class UUserWidget;
/**
 * 
 */
UCLASS()
class TAG_API ATagPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaSeconds) override;

	void ShowScoreboard();
	void HideScoreboard();
	void ShowPauseMenu();
	void HidePauseMenu();

protected:
	virtual void BeginPlay() override;
	virtual void AcknowledgePossession(APawn* P) override;
	virtual void SetupInputComponent() override;

	//Scoreboard
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Controls|Input Actions")
	UInputMappingContext* BaseMappingContext;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Controls|Input Actions")
	int32 BaseMappingPriority = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Controls|Input Actions")
	UInputAction* ScoreboardAction;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UScoreboard> ScoreboardClass;

	//Pause Menu
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPauseMenu> PauseMenuClass;
	UPROPERTY(EditDefaultsOnly, Category = "Controls|Input Actions")
	UInputAction* PauseMenuAction;
	
private:
	UPROPERTY()
	UScoreboard* ScoreboardRef;
	UPROPERTY()
	UPauseMenu* PauseMenuRef;
	
};

