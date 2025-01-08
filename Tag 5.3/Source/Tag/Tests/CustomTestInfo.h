// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GenericTestInfo.h"
#include "Evidence/Character/BaseCharacter.h"
#include "Evidence/Game/EvidenceGameState.h"
#include "Evidence/Game/EvidenceGameMode.h"
#include "Evidence/Player/EvidencePlayerController.h"
#include "Evidence/Player/EvidencePlayerState.h"
#include "TestPlayerCount.h"
#include "CustomTestInfo.generated.h"

USTRUCT(BlueprintType)
struct FCustomPlayerTestInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AEvidencePlayerController> MyPlayerController = nullptr;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ABaseCharacter> ServerCharacter = nullptr;

	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<ABaseCharacter>> ClientCharacters;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AEvidencePlayerState> ServerPlayerState = nullptr;

	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<AEvidencePlayerState>> ClientPlayerStates;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> MyInputSubsystem = nullptr;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AEvidenceGameState> MyGameState = nullptr;

	FCustomPlayerTestInfo() = default;

	FCustomPlayerTestInfo(const FGenericPlayerTestInfo& TestInfo)
	{
		MyPlayerController = Cast<AEvidencePlayerController>(TestInfo.MyPlayerController);
		ServerCharacter = Cast<ABaseCharacter>(TestInfo.ServerPawn);
		ServerPlayerState = Cast<AEvidencePlayerState>(TestInfo.ServerPlayerState);
		MyInputSubsystem = TestInfo.MyInputSubsystem;
		MyGameState = Cast<AEvidenceGameState>(TestInfo.MyGameState);

		for (TObjectPtr<APawn> Pawn : TestInfo.ClientPawns)
		{
			ClientCharacters.Add(Cast<ABaseCharacter>(Pawn));
		}

		for (TObjectPtr<APlayerState> PlayerState : TestInfo.ClientPlayerStates)
		{
			ClientPlayerStates.Add(Cast<AEvidencePlayerState>(PlayerState));
		}

		TestPlayerCount = TestInfo.GetTestPlayerCount();
	}

	virtual ~FCustomPlayerTestInfo()
	{

	}

	virtual TObjectPtr<ABaseCharacter> MyCharacter()
	{
		return nullptr;
	}

	virtual TObjectPtr<AEvidencePlayerState> MyPlayerState()
	{
		return nullptr;
	}

	TObjectPtr<ABaseCharacter> GetClient1Character()
	{
		checkf(TestPlayerCount >= ETestPlayerCount::ServerAnd1Client, TEXT("Test doesn't have 1 client"));

		return ClientCharacters[0];
	}

	TObjectPtr<ABaseCharacter> GetClient2Character()
	{
		checkf(TestPlayerCount >= ETestPlayerCount::ServerAnd2Clients, TEXT("Test doesn't have 2 clients"));

		return ClientCharacters[1];
	}

	TObjectPtr<ABaseCharacter> GetClient3Character()
	{
		checkf(TestPlayerCount >= ETestPlayerCount::ServerAnd3Clients, TEXT("Test doesn't have 3 clients"));

		return ClientCharacters[2];
	}

private:
	ETestPlayerCount TestPlayerCount = ETestPlayerCount::ServerAnd2Clients;
};

USTRUCT(BlueprintType)
struct FCustomServerTestInfo : public FCustomPlayerTestInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AEvidenceGameMode> MyGameMode = nullptr;

	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<AEvidencePlayerController>> ClientPlayerControllers;

	FCustomServerTestInfo() = default;

	FCustomServerTestInfo(const FGenericServerTestInfo& TestInfo)
		: FCustomPlayerTestInfo(TestInfo)
	{
		MyGameMode = Cast<AEvidenceGameMode>(TestInfo.MyGameMode);
		for (TObjectPtr<APlayerController> PlayerController : TestInfo.ClientPlayerControllers)
		{
			ClientPlayerControllers.Add(Cast<AEvidencePlayerController>(PlayerController));
		}
	}

	TObjectPtr<ABaseCharacter> MyCharacter() override
	{
		return ServerCharacter;
	}

	TObjectPtr<AEvidencePlayerState> MyPlayerState() override
	{
		return ServerPlayerState;
	}
};

USTRUCT(BlueprintType)
struct FCustomClientTestInfo : public FCustomPlayerTestInfo
{
	GENERATED_BODY()

public:
	FCustomClientTestInfo() = default;

	FCustomClientTestInfo(const FGenericClientTestInfo& TestInfo)
		: FCustomPlayerTestInfo(TestInfo), MyIndex(TestInfo.GetIndex())
	{
		
	}

	TObjectPtr<ABaseCharacter> MyCharacter() override
	{
		return ClientCharacters[MyIndex];
	}

	TObjectPtr<AEvidencePlayerState> MyPlayerState() override
	{
		return ClientPlayerStates[MyIndex];
	}

private:
	uint8 MyIndex = 0;
};