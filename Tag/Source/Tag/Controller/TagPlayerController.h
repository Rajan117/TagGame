// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TagPlayerController.generated.h"

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
	
	virtual float GetServerTime(); //Synced with server

	virtual void Tick(float DeltaSeconds) override;
	virtual void ReceivedPlayer() override;
	
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
	
private:
	UPROPERTY()
	ATagHUD* TagHUD;

	float MatchTime = 120.f;
	uint32 TimerInt = 0;
};
