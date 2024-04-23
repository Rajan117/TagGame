// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Settings/EditorLoadingSavingSettings.h"
#include "TagPlayerController.generated.h"

class UGameStartTimer;
class ATagHUD;


/**
 * 
 */
UCLASS()
class TAG_API ATagPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	void SetCurrentEffectHUD(const FString& EffectText);
	void SetHUDTimerText(const float Time);
	void SetScoreTextHUD(const float Score);
	
	virtual float GetServerTime(); //Synced with server

	virtual void Tick(float DeltaSeconds) override;
	virtual void ReceivedPlayer() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void OnMatchStateSet(FName State);
	
protected:
	virtual void BeginPlay() override;
	virtual void AcknowledgePossession(APawn* P) override;
	void SetHUDTime();

	/**
	 * Sync Client and Server Time
	 */
	UFUNCTION(Server, Reliable) //Request current server time, passing client's time when the request is made
	void ServerRequestServerTime(float TimeOfClientRequest);
	UFUNCTION(Client, Reliable) //Sends current server time to client
	void ClientReportServerTime(float TimeOfClientRequest, float TimeServerReceivedClientRequest);
	float ClientServerDelta = 0; //Difference between client and server time
	UPROPERTY(EditAnywhere, Category="Time")
	float TimeSyncFrequency = 5.f; //Frequency to sync client's time to server's time
	float TimeSyncRunningTime = 0;
	void CheckTimeSync(float DeltaSeconds);

	virtual void StartGameStartCountdown();
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UGameStartTimer> GameStartTimerClass;

	UFUNCTION(Server, Reliable)
	void ServerCheckMatchState();
	UFUNCTION(Client, Reliable)
	void ClientJoinMidgame(
		FName StateOfMatch,
		float Warmup,
		float Match,
		float LevelStart,
		float RoundStart
	);
	
private:
	UPROPERTY()
	ATagHUD* TagHUD;

	float MatchTime = 0.f;
	float WarmupTime = 0.f;
	float LevelStartingTime = 0.f;
	float RoundStartingTime = 0.f;
	uint32 TimerInt = 0;

	UPROPERTY(ReplicatedUsing=OnRep_MatchState)
	FName MatchState;
	UFUNCTION()
	void OnRep_MatchState();
};

