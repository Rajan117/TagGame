// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Settings/EditorLoadingSavingSettings.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "TagPlayerController.generated.h"

class ATagGameState;
class UScoreboard;
class ATagHUD;
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
	void AddHUDTagAnnouncement(const FString& Tagger, const FString& Tagged);
	void ShowScoreboard();
	void HideScoreboard();
	
	virtual void Tick(float DeltaSeconds) override;
	virtual void ReceivedPlayer() override;

	UFUNCTION()
	void OnMatchStateSet(FName State);

	void BroadcastTag(ATagPlayerState* TaggingPLayer, ATagPlayerState* TaggedPlayer);
	
protected:
	virtual void BeginPlay() override;
	virtual void AcknowledgePossession(APawn* P) override;
	virtual void SetupInputComponent() override;

	UFUNCTION(Client, Reliable)
	void ClientTagAnnouncement(ATagPlayerState* TaggingPlayer, ATagPlayerState* TaggedPlayer);

	//Scoreboard
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Controls|Input Actions")
	UInputMappingContext* BaseMappingContext;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Controls|Input Actions")
	int32 BaseMappingPriority = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Controls|Input Actions")
	UInputAction* ScoreboardAction;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UScoreboard> ScoreboardClass;
	
private:
	//References
	UPROPERTY()
	ATagHUD* TagHUD;
	UPROPERTY()
	ATagGameState* TagGameState;
	UPROPERTY()
	UScoreboard* ScoreboardRef;

	UPROPERTY()
	FName MatchState;
	bool bInitialisedHUD = false;

public:
	FORCEINLINE ATagGameState* GetTagGameState() const { return TagGameState; }

};

