// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/Pawn.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "TestPlayerCount.h"

struct FGenericPlayerTestInfo
{
public:
	APlayerController* MyPlayerController = nullptr;
	APawn* ServerPawn = nullptr;
	TArray<APawn*> ClientPawns;
	APlayerState* ServerPlayerState = nullptr;
	TArray<APlayerState*> ClientPlayerStates;
	UEnhancedInputLocalPlayerSubsystem* MyInputSubsystem = nullptr;
	AGameStateBase* MyGameState = nullptr;

	FGenericPlayerTestInfo()
		: FGenericPlayerTestInfo(ETestPlayerCount::ServerAnd3Clients)
	{

	}

	FGenericPlayerTestInfo(const ETestPlayerCount PlayerCount)
		: TestPlayerCount(PlayerCount)
	{
		uint8 Count = GetCount(PlayerCount);

		ClientPawns.SetNum(Count);
		ClientPlayerStates.SetNum(Count);
	}

	ETestPlayerCount GetTestPlayerCount() const
	{
		return TestPlayerCount;
	}

private:
	ETestPlayerCount TestPlayerCount;
};

struct FGenericServerTestInfo : public FGenericPlayerTestInfo
{
public:
	AGameModeBase* MyGameMode = nullptr;
	TArray<APlayerController*> ClientPlayerControllers;

	FGenericServerTestInfo()
		: FGenericServerTestInfo(ETestPlayerCount::ServerAnd3Clients)
	{

	}

	FGenericServerTestInfo(const ETestPlayerCount PlayerCount)
		: FGenericPlayerTestInfo(PlayerCount)
	{
		uint8 Count = GetCount(PlayerCount);

		ClientPlayerControllers.SetNum(Count);
	}
};

struct FGenericClientTestInfo : public FGenericPlayerTestInfo
{
public:
	FGenericClientTestInfo()
	{

	}

	FGenericClientTestInfo(const ETestPlayerCount PlayerCount, const uint8 Index)
		: FGenericPlayerTestInfo(PlayerCount), MyIndex(Index)
	{

	}

	APawn* MyPawn() const
	{
		return ClientPawns[MyIndex];
	}

	APlayerState* MyPlayerState() const
	{
		return ClientPlayerStates[MyIndex];
	}

	void SetMyPawn(APawn* Pawn)
	{
		ClientPawns[MyIndex] = Pawn;
	}

	void SetMyPlayerState(APlayerState* PlayerState)
	{
		ClientPlayerStates[MyIndex] = PlayerState;
	}

	uint8 GetIndex() const
	{
		return MyIndex;
	}

private:
	uint8 MyIndex;
};