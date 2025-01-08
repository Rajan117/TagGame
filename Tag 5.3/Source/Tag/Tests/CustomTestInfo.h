// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GenericTestInfo.h"
#include "Tag/Character/TagCharacter.h"
#include "Tag/GameStates/TagGameState.h"
#include "Tag/GameModes/TagGameMode.h"
#include "Tag/Controller/TagPlayerController.h"
#include "Tag/PlayerState/TagPlayerState.h"
#include "TestPlayerCount.h"
#include "CustomTestInfo.generated.h"

USTRUCT(BlueprintType)
struct FCustomPlayerTestInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ATagPlayerController> MyPlayerController = nullptr;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ATagCharacter> ServerCharacter = nullptr;

	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<ATagCharacter>> ClientCharacters;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ATagPlayerState> ServerPlayerState = nullptr;

	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<ATagPlayerState>> ClientPlayerStates;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> MyInputSubsystem = nullptr;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ATagGameState> MyGameState = nullptr;

	FCustomPlayerTestInfo() = default;

	FCustomPlayerTestInfo(const FGenericPlayerTestInfo& TestInfo)
	{
		MyPlayerController = Cast<ATagPlayerController>(TestInfo.MyPlayerController);
		ServerCharacter = Cast<ATagCharacter>(TestInfo.ServerPawn);
		ServerPlayerState = Cast<ATagPlayerState>(TestInfo.ServerPlayerState);
		MyInputSubsystem = TestInfo.MyInputSubsystem;
		MyGameState = Cast<ATagGameState>(TestInfo.MyGameState);

		for (TObjectPtr<APawn> Pawn : TestInfo.ClientPawns)
		{
			ClientCharacters.Add(Cast<ATagCharacter>(Pawn));
		}

		for (TObjectPtr<APlayerState> PlayerState : TestInfo.ClientPlayerStates)
		{
			ClientPlayerStates.Add(Cast<ATagPlayerState>(PlayerState));
		}

		TestPlayerCount = TestInfo.GetTestPlayerCount();
	}

	virtual ~FCustomPlayerTestInfo()
	{

	}

	virtual TObjectPtr<ATagCharacter> MyCharacter()
	{
		return nullptr;
	}

	virtual TObjectPtr<ATagPlayerState> MyPlayerState()
	{
		return nullptr;
	}

	TObjectPtr<ATagCharacter> GetClient1Character()
	{
		checkf(TestPlayerCount >= ETestPlayerCount::ServerAnd1Client, TEXT("Test doesn't have 1 client"));

		return ClientCharacters[0];
	}

	TObjectPtr<ATagCharacter> GetClient2Character()
	{
		checkf(TestPlayerCount >= ETestPlayerCount::ServerAnd2Clients, TEXT("Test doesn't have 2 clients"));

		return ClientCharacters[1];
	}

	TObjectPtr<ATagCharacter> GetClient3Character()
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
	TObjectPtr<ATagGameMode> MyGameMode = nullptr;

	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<ATagPlayerController>> ClientPlayerControllers;

	FCustomServerTestInfo() = default;

	FCustomServerTestInfo(const FGenericServerTestInfo& TestInfo)
		: FCustomPlayerTestInfo(TestInfo)
	{
		MyGameMode = Cast<ATagGameMode>(TestInfo.MyGameMode);
		for (TObjectPtr<APlayerController> PlayerController : TestInfo.ClientPlayerControllers)
		{
			ClientPlayerControllers.Add(Cast<ATagPlayerController>(PlayerController));
		}
	}

	TObjectPtr<ATagCharacter> MyCharacter() override
	{
		return ServerCharacter;
	}

	TObjectPtr<ATagPlayerState> MyPlayerState() override
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

	TObjectPtr<ATagCharacter> MyCharacter() override
	{
		return ClientCharacters[MyIndex];
	}

	TObjectPtr<ATagPlayerState> MyPlayerState() override
	{
		return ClientPlayerStates[MyIndex];
	}

private:
	uint8 MyIndex = 0;
};