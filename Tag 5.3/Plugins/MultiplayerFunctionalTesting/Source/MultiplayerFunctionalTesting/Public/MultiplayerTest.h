// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FunctionalTest.h"
#include "ClientWorldFunctionLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/TriggerBox.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GenericTestInfo.h"
#include "MultiplayerTest.generated.h"

class UInputAction;

/**
 * 
 */
UCLASS()
class MULTIPLAYERFUNCTIONALTESTING_API AMultiplayerTest : public AFunctionalTest
{
	GENERATED_BODY()

public:
	AMultiplayerTest();
	void StartTest() override;
	bool IsReady_Implementation() override;

	ETestPlayerCount GetTestPlayerCount() const { return TestPlayerCount; }

protected:
	UFUNCTION(BlueprintNativeEvent)
	void ConvertGenericServerTestInfo();
	virtual void ConvertGenericServerTestInfo_Implementation() {};

	UFUNCTION(BlueprintNativeEvent)
	void ConvertGenericClientTestInfos();
	virtual void ConvertGenericClientTestInfos_Implementation() {};

	bool bTestHasBegun;

	void BindTriggers();

	UFUNCTION()
	virtual void OnServerTriggerBeginOverlap(AActor* OverlappedActor, AActor* OtherActor) {};
	UFUNCTION()
	virtual void OnClient1TriggerBeginOverlap(AActor* OverlappedActor, AActor* OtherActor) {};
	UFUNCTION()
	virtual void OnClient2TriggerBeginOverlap(AActor* OverlappedActor, AActor* OtherActor) {};
	UFUNCTION()
	virtual void OnClient3TriggerBeginOverlap(AActor* OverlappedActor, AActor* OtherActor) {};

	const FGenericServerTestInfo& GetGenericServerTestInfo() const { return GenericServerTestInfo; }
	const TArray<FGenericClientTestInfo> GetGenericClientTestInfos() const { return GenericClientTestInfos; }

	void CheckResult();

	bool bServerPassed;
	bool bClient1Passed;
	bool bClient2Passed;
	bool bClient3Passed;

	void InjectInput(UEnhancedInputLocalPlayerSubsystem* InputSubsystem, UInputAction* Action, const FVector& Value = FVector(1, 0, 0));
	void StartInjectContinuousInput(UEnhancedInputLocalPlayerSubsystem* InputSubsystem, UInputAction* Action, const FVector& Value = FVector(1, 0, 0));
	void StopInjectContinuousInput(UEnhancedInputLocalPlayerSubsystem* InputSubsystem, UInputAction* Action);

private:
	UPROPERTY(EditDefaultsOnly)
	ETestPlayerCount TestPlayerCount = ETestPlayerCount::ServerAnd2Clients;

	FGenericServerTestInfo GenericServerTestInfo;
	TArray<FGenericClientTestInfo> GenericClientTestInfos;

	void ServerGetOtherPlayers();
	bool IsServerReady();
	void ClientGetOtherPlayers(const uint8 ClientIndex, FGenericClientTestInfo& ClientTestInfo) const;
	bool IsClientReady(const uint8 ClientIndex, FGenericClientTestInfo& ClientTestInfo) const;

	bool IsGameModeReady();
	bool IsServerGameStateReady();
	bool IsServerPlayerControllerReady();
	bool IsServerPlayerStateReady();
	bool IsServerInputReady();
	bool IsServerPawnReady();
	bool ServerAreOtherPlayersReady();

	bool IsClientGameStateReady(const uint8 ClientIndex, FGenericClientTestInfo& ClientTestInfo) const;
	bool IsClientPlayerControllerReady(const uint8 ClientIndex, FGenericClientTestInfo& ClientTestInfo) const;
	bool IsClientPlayerStateReady(const uint8 ClientIndex, FGenericClientTestInfo& ClientTestInfo) const;
	bool IsClientInputReady(const uint8 ClientIndex, FGenericClientTestInfo& ClientTestInfo) const;
	bool IsClientPawnReady(const uint8 ClientIndex, FGenericClientTestInfo& ClientTestInfo) const;
	bool ClientAreOtherPlayersReady(const uint8 ClientIndex) const;

	UPROPERTY()
	ATriggerBox* ServerTriggerBox;

	UPROPERTY()
	ATriggerBox* Client1TriggerBox;

	UPROPERTY()
	ATriggerBox* Client2TriggerBox;

	UPROPERTY()
	ATriggerBox* Client3TriggerBox;
};
