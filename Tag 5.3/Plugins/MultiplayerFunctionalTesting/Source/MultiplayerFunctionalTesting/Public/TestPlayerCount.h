// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TestPlayerCount.generated.h"

UENUM(BlueprintType)
enum class ETestPlayerCount : uint8
{
	ServerOnly			UMETA(DisplayName = "Server Only"),
	ServerAnd1Client	UMETA(DisplayName = "Server & 1 Client"),
	ServerAnd2Clients	UMETA(DisplayName = "Server & 2 Clients"),
	ServerAnd3Clients	UMETA(DisplayName = "Server & 3 Clients")
};

FORCEINLINE uint8 GetCount(const ETestPlayerCount PlayerCount)
{
	uint8 Count = 0;

	switch (PlayerCount)
	{
	case ETestPlayerCount::ServerOnly:
		Count = 1;
		break;
	case ETestPlayerCount::ServerAnd1Client:
		Count = 2;
		break;
	case ETestPlayerCount::ServerAnd2Clients:
		Count = 3;
		break;
	case ETestPlayerCount::ServerAnd3Clients:
		Count = 4;
		break;
	}

	return Count;
}