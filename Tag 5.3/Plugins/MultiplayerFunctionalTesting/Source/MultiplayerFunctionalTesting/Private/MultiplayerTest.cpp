// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerTest.h"

AMultiplayerTest::AMultiplayerTest()
{
	LogErrorHandling = EFunctionalTestLogHandling::OutputIgnored;
	LogWarningHandling = EFunctionalTestLogHandling::OutputIgnored;

	bTestHasBegun = false;

	bServerPassed = false;
	bClient1Passed = true;
	bClient2Passed = true;
	bClient3Passed = true;

	GenericServerTestInfo = FGenericServerTestInfo(TestPlayerCount);
	
	for (uint8 Index = 0; Index < GetCount(TestPlayerCount) - 1; Index++)
	{
		GenericClientTestInfos.Add(FGenericClientTestInfo(TestPlayerCount, Index));
	}
}

void AMultiplayerTest::StartTest()
{
	Super::StartTest();

	ServerGetOtherPlayers();

	for (uint8 Index = 0; Index < GetCount(TestPlayerCount) - 1; Index++)
	{
		ClientGetOtherPlayers(Index, GenericClientTestInfos[Index]);
	}

	ConvertGenericServerTestInfo();
	ConvertGenericClientTestInfos();
}

bool AMultiplayerTest::IsReady_Implementation()
{
	bool bReady = true;

	bReady &= IsServerReady();

	for (uint8 Index = 0; Index < GetCount(TestPlayerCount) - 1; Index++)
	{
		bReady &= IsClientReady(Index, GenericClientTestInfos[Index]);
	}

	if (bReady)
	{
		bTestHasBegun = true;
	}

	return bReady;
}

bool AMultiplayerTest::IsServerReady()
{
	bool bSuccess = true;

	bSuccess &= IsGameModeReady();
	bSuccess &= IsServerGameStateReady();
	bSuccess &= IsServerPlayerControllerReady();
	bSuccess &= IsServerPlayerStateReady();
	bSuccess &= IsServerInputReady();
	bSuccess &= IsServerPawnReady();
	bSuccess &= ServerAreOtherPlayersReady();

	return bSuccess;
}

bool AMultiplayerTest::IsClientReady(const uint8 ClientIndex, FGenericClientTestInfo& ClientTestInfo) const
{
	bool bSuccess = true;

	bSuccess &= IsClientGameStateReady(ClientIndex, ClientTestInfo);
	bSuccess &= IsClientPlayerControllerReady(ClientIndex, ClientTestInfo);
	bSuccess &= IsClientPlayerStateReady(ClientIndex, ClientTestInfo);
	bSuccess &= IsClientInputReady(ClientIndex, ClientTestInfo);
	bSuccess &= IsClientPawnReady(ClientIndex, ClientTestInfo);
	bSuccess &= ClientAreOtherPlayersReady(ClientIndex);

	return bSuccess;
}

void AMultiplayerTest::ServerGetOtherPlayers()
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerState::StaticClass(), Actors);

	for (AActor* Actor : Actors)
	{
		APlayerState* PlayerState = Cast<APlayerState>(Actor);

		for (uint8 Index = 0; Index < GetCount(TestPlayerCount) - 1; Index++)
		{
			if (PlayerState->GetPlayerId() == GenericClientTestInfos[Index].MyPlayerState()->GetPlayerId())
			{
				GenericServerTestInfo.ClientPawns[Index] = PlayerState->GetPawn();
				GenericServerTestInfo.ClientPlayerStates[Index] = PlayerState;
				GenericServerTestInfo.ClientPlayerControllers[Index] = PlayerState->GetPlayerController();
			}
		}
	}
}

void AMultiplayerTest::ClientGetOtherPlayers(const uint8 PlayerIndex, FGenericClientTestInfo& ClientTestInfo) const
{
	TArray<FGenericClientTestInfo> OtherClients;
	for (const FGenericClientTestInfo& TestInfo : GenericClientTestInfos)
	{
		OtherClients.Add(TestInfo);
	}

	TArray<AActor*> Actors;
	UClientWorldFunctionLibrary::GetAllClientActorsOfClass(APlayerState::StaticClass(), Actors, PlayerIndex);

	for (AActor* Actor : Actors)
	{
		APlayerState* PlayerState = Cast<APlayerState>(Actor);

		if (PlayerState->GetPlayerId() == GenericServerTestInfo.ServerPlayerState->GetPlayerId())
		{
			ClientTestInfo.ServerPawn = PlayerState->GetPawn();
			ClientTestInfo.ServerPlayerState = PlayerState;
		}

		for (const FGenericClientTestInfo& OtherClientTestInfo : OtherClients)
		{
			if (PlayerState->GetPlayerId() == OtherClientTestInfo.MyPlayerState()->GetPlayerId())
			{
				ClientTestInfo.ClientPawns[OtherClientTestInfo.GetIndex()] = PlayerState->GetPawn();
				ClientTestInfo.ClientPlayerStates[OtherClientTestInfo.GetIndex()] = PlayerState;
			}
		}
	}
}

void AMultiplayerTest::BindTriggers()
{
	ServerTriggerBox = Cast<ATriggerBox>(UGameplayStatics::GetActorOfClass(GetWorld(), ATriggerBox::StaticClass()));
	if (ServerTriggerBox)
	{
		ServerTriggerBox->OnActorBeginOverlap.AddDynamic(this, &ThisClass::OnServerTriggerBeginOverlap);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ServerTriggerBox is invalid"))
	}

	Client1TriggerBox = Cast<ATriggerBox>(UClientWorldFunctionLibrary::GetClientActorOfClass(ATriggerBox::StaticClass(), 0));
	if (Client1TriggerBox)
	{
		Client1TriggerBox->OnActorBeginOverlap.AddDynamic(this, &ThisClass::OnClient1TriggerBeginOverlap);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Client1TriggerBox is invalid"))
	}

	Client2TriggerBox = Cast<ATriggerBox>(UClientWorldFunctionLibrary::GetClientActorOfClass(ATriggerBox::StaticClass(), 1));
	if (Client2TriggerBox)
	{
		Client2TriggerBox->OnActorBeginOverlap.AddDynamic(this, &ThisClass::OnClient2TriggerBeginOverlap);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Client2TriggerBox is invalid"))
	}

	Client2TriggerBox = Cast<ATriggerBox>(UClientWorldFunctionLibrary::GetClientActorOfClass(ATriggerBox::StaticClass(), 1));
	if (Client2TriggerBox)
	{
		Client2TriggerBox->OnActorBeginOverlap.AddDynamic(this, &ThisClass::OnClient2TriggerBeginOverlap);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Client2TriggerBox is invalid"))
	}

	Client3TriggerBox = Cast<ATriggerBox>(UClientWorldFunctionLibrary::GetClientActorOfClass(ATriggerBox::StaticClass(), 2));
	if (Client3TriggerBox)
	{
		Client3TriggerBox->OnActorBeginOverlap.AddDynamic(this, &ThisClass::OnClient3TriggerBeginOverlap);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Client3TriggerBox is invalid"))
	}
}

void AMultiplayerTest::CheckResult()
{
	if (bServerPassed && bClient1Passed && bClient2Passed && bClient3Passed)
	{
		FinishTest(EFunctionalTestResult::Succeeded, FString("Passed"));
	}
}

void AMultiplayerTest::InjectInput(UEnhancedInputLocalPlayerSubsystem* InputSubsystem, UInputAction* Action, const FVector& Value)
{
	TArray<UInputModifier*> Modifiers = {};
	TArray<UInputTrigger*> Triggers = {};

	InputSubsystem->InjectInputForAction(Action, Value, Modifiers, Triggers);
}

void AMultiplayerTest::StartInjectContinuousInput(UEnhancedInputLocalPlayerSubsystem* InputSubsystem, UInputAction* Action, const FVector& Value)
{
	TArray<UInputModifier*> Modifiers = {};
	TArray<UInputTrigger*> Triggers = {};

	InputSubsystem->StartContinuousInputInjectionForAction(Action, Value, Modifiers, Triggers);
}

void AMultiplayerTest::StopInjectContinuousInput(UEnhancedInputLocalPlayerSubsystem* InputSubsystem, UInputAction* Action)
{
	TArray<UInputModifier*> Modifiers = {};
	TArray<UInputTrigger*> Triggers = {};

	InputSubsystem->StopContinuousInputInjectionForAction(Action);
}

bool AMultiplayerTest::IsGameModeReady()
{
	bool bSuccess = false;

	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());
	if (GameMode)
	{
		bSuccess = true;

		GenericServerTestInfo.MyGameMode = GameMode;
	}

	return bSuccess;
}

bool AMultiplayerTest::IsServerGameStateReady()
{
	bool bSuccess = false;

	AGameStateBase* GameState = UGameplayStatics::GetGameState(GetWorld());
	if (GameState)
	{
		bSuccess = true;

		GenericServerTestInfo.MyGameState = GameState;
	}

	return bSuccess;
}

bool AMultiplayerTest::IsServerPlayerControllerReady()
{
	bool bSuccess = false;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		bSuccess = true;

		GenericServerTestInfo.MyPlayerController = PlayerController;
	}

	return bSuccess;
}

bool AMultiplayerTest::IsServerPlayerStateReady()
{
	bool bSuccess = false;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		APlayerState* const PlayerState = PlayerController->PlayerState;

		if (PlayerState)
		{
			bSuccess = true;

			GenericServerTestInfo.ServerPlayerState = PlayerState;
		}
	}

	return bSuccess;
}

bool AMultiplayerTest::IsServerInputReady()
{
	bool bSuccess = false;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* const InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		if (InputSubsystem)
		{
			bSuccess = true;

			GenericServerTestInfo.MyInputSubsystem = InputSubsystem;
		}
	}

	return bSuccess;
}

bool AMultiplayerTest::IsServerPawnReady()
{
	bool bSuccess = false;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		APawn* const Pawn = Cast<APawn>(PlayerController->GetPawn());

		if (Pawn)
		{
			bSuccess = true;

			GenericServerTestInfo.ServerPawn = Pawn;
		}
	}

	return bSuccess;
}

bool AMultiplayerTest::ServerAreOtherPlayersReady()
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerState::StaticClass(), Actors);
	uint8 ReadyCount = 0;
	for (AActor* Actor : Actors)
	{
		APlayerState* PlayerState = Cast<APlayerState>(Actor);

		if (PlayerState)
		{
			APawn* Pawn = PlayerState->GetPawn();

			if (Pawn)
			{
				ReadyCount++;
			}
		}
	}

	return ReadyCount == GetCount(TestPlayerCount);
}

bool AMultiplayerTest::IsClientGameStateReady(const uint8 ClientIndex, FGenericClientTestInfo& ClientTestInfo) const
{
	bool bSuccess = false;

	AGameStateBase* GameState = Cast<AGameStateBase>(UClientWorldFunctionLibrary::GetClientActorOfClass(AGameStateBase::StaticClass(), ClientIndex));
	if (GameState)
	{
		bSuccess = true;

		ClientTestInfo.MyGameState = GameState;
	}

	return bSuccess;
}

bool AMultiplayerTest::IsClientPlayerControllerReady(const uint8 ClientIndex, FGenericClientTestInfo& ClientTestInfo) const
{
	bool bSuccess = false;

	APlayerController* PlayerController = Cast<APlayerController>(UClientWorldFunctionLibrary::GetClientActorOfClass(APlayerController::StaticClass(), ClientIndex));
	if (PlayerController)
	{
		bSuccess = true;

		ClientTestInfo.MyPlayerController = PlayerController;
	}

	return bSuccess;
}

bool AMultiplayerTest::IsClientPlayerStateReady(const uint8 ClientIndex, FGenericClientTestInfo& ClientTestInfo) const
{
	bool bSuccess = false;

	APlayerController* PlayerController = Cast<APlayerController>(UClientWorldFunctionLibrary::GetClientActorOfClass(APlayerController::StaticClass(), ClientIndex));
	if (PlayerController)
	{
		APlayerState* const PlayerState = PlayerController->PlayerState;

		if (PlayerState)
		{
			bSuccess = true;

			ClientTestInfo.SetMyPlayerState(PlayerState);
		}
	}

	return bSuccess;
}

bool AMultiplayerTest::IsClientInputReady(const uint8 ClientIndex, FGenericClientTestInfo& ClientTestInfo) const
{
	bool bSuccess = false;

	APlayerController* PlayerController = Cast<APlayerController>(UClientWorldFunctionLibrary::GetClientActorOfClass(APlayerController::StaticClass(), ClientIndex));
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* const InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		if (InputSubsystem)
		{
			bSuccess = true;

			ClientTestInfo.MyInputSubsystem = InputSubsystem;
		}
	}

	return bSuccess;
}

bool AMultiplayerTest::IsClientPawnReady(const uint8 ClientIndex, FGenericClientTestInfo& ClientTestInfo) const
{
	bool bSuccess = false;

	APlayerController* PlayerController = Cast<APlayerController>(UClientWorldFunctionLibrary::GetClientActorOfClass(APlayerController::StaticClass(), ClientIndex));
	if (PlayerController)
	{
		APawn* const Pawn = Cast<APawn>(PlayerController->GetPawn());

		if (Pawn)
		{
			bSuccess = true;

			ClientTestInfo.SetMyPawn(Pawn);
		}
	}

	return bSuccess;
}

bool AMultiplayerTest::ClientAreOtherPlayersReady(const uint8 ClientIndex) const
{
	TArray<AActor*> Actors;
	UClientWorldFunctionLibrary::GetAllClientActorsOfClass(APlayerState::StaticClass(), Actors, ClientIndex);
	uint8 ReadyCount = 0;
	for (AActor* Actor : Actors)
	{
		APlayerState* PlayerState = Cast<APlayerState>(Actor);

		if (PlayerState)
		{
			APawn* Pawn = PlayerState->GetPawn();

			if (Pawn)
			{
				ReadyCount++;
			}
		}
	}

	return ReadyCount == GetCount(TestPlayerCount);
}
