// Fill out your copyright notice in the Description page of Project Settings.


#include "TagPlayerController.h"


#include "Tag/Character/TagCharacter.h"
#include "Tag/HUD/CharacterOverlay.h"
#include "Tag/HUD/TagHUD.h"
#include "Tag/HUD/HUDElements/GameTimer.h"
#include "Tag/HUD/HUDElements/GameStartTimer.h"
#include "Tag/GameModes/TagGameMode.h"

#include "Components/TextBlock.h"
#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

void ATagPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TagHUD = TagHUD == nullptr ? Cast<ATagHUD>(GetHUD()) : TagHUD;
	ServerCheckMatchState();
}

void ATagPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATagPlayerController, MatchState);
}

void ATagPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	SetHUDTime();

	CheckTimeSync(DeltaSeconds);
}

void ATagPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();

	if (IsLocalController())
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
	}
}

void ATagPlayerController::OnMatchStateSet(const FName State)
{
	MatchState = State;

	if (MatchState == MatchState::Warmup)
	{
		TagHUD = TagHUD == nullptr ? Cast<ATagHUD>(GetHUD()) : TagHUD;
		if (TagHUD) TagHUD->AddCharacterOverlay();
		StartGameStartCountdown();
	}
}

void ATagPlayerController::OnRep_MatchState()
{
	if (MatchState == MatchState::Warmup)
	{
		TagHUD = TagHUD == nullptr ? Cast<ATagHUD>(GetHUD()) : TagHUD;
		if (TagHUD) TagHUD->AddCharacterOverlay();
		StartGameStartCountdown();
	}
}

void ATagPlayerController::StartGameStartCountdown()
{
	if (GameStartTimerClass)
	{
		if (UGameStartTimer* GameStartTimer = CreateWidget<UGameStartTimer>(GetWorld(), GameStartTimerClass))
		{
			GameStartTimer->WarmupTime = WarmupTime;
			GameStartTimer->AddToViewport();
		}
	}
}

void ATagPlayerController::ServerCheckMatchState_Implementation()
{
	if (ATagGameMode* TagGameMode = Cast<ATagGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		WarmupTime = TagGameMode->WarmupTime;
		MatchTime = TagGameMode->MatchTime;
		LevelStartingTime = TagGameMode->LevelStartingTime;
		RoundStartingTime = TagGameMode->RoundStartingTime;
		MatchState = TagGameMode->GetMatchState();
		ClientJoinMidgame(MatchState, WarmupTime, MatchTime, LevelStartingTime, RoundStartingTime);
	}
}

void ATagPlayerController::ClientJoinMidgame_Implementation(FName StateOfMatch, float Warmup, float Match,
	float LevelStart, float RoundStart)
{
	WarmupTime = Warmup;
	MatchTime = Match;
	LevelStartingTime =  LevelStart;
	RoundStartingTime = RoundStart;
	MatchState = StateOfMatch;
	OnMatchStateSet(MatchState);
}

void ATagPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);

	ATagCharacter* TagCharacter = Cast<ATagCharacter>(P);
	if (TagCharacter)
	{
		TagCharacter->GetAbilitySystemComponent()->InitAbilityActorInfo(TagCharacter, TagCharacter);
	}
}

void ATagPlayerController::SetHUDTime()
{
	uint32 SecondsLeft = MatchTime;
	if (MatchState == MatchState::InMatch) SecondsLeft = FMath::CeilToInt(WarmupTime+MatchTime-GetServerTime()+RoundStartingTime+3);
	if (TimerInt != SecondsLeft)
	{
		SetHUDTimerText(SecondsLeft);
	}
	TimerInt = SecondsLeft;
}

#pragma region Time Syncing

float ATagPlayerController::GetServerTime()
{
	if (HasAuthority()) return GetWorld()->GetTimeSeconds();
	return GetWorld()->GetTimeSeconds() + ClientServerDelta;
}

void ATagPlayerController::ServerRequestServerTime_Implementation(const float TimeOfClientRequest)
{
	const float ServerTimeOfReceipt = GetWorld()->GetTimeSeconds();
	ClientReportServerTime(TimeOfClientRequest, ServerTimeOfReceipt);
}

void ATagPlayerController::ClientReportServerTime_Implementation(const float TimeOfClientRequest,
                                                                 const float TimeServerReceivedClientRequest)
{
	const float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfClientRequest;
	const float CurrentServerTime = TimeServerReceivedClientRequest + (RoundTripTime * 0.5f);
	ClientServerDelta = CurrentServerTime - GetWorld()->GetTimeSeconds();
}

void ATagPlayerController::CheckTimeSync(const float DeltaSeconds)
{
	TimeSyncRunningTime += DeltaSeconds;
	if (IsLocalController() && TimeSyncRunningTime > TimeSyncFrequency)
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
	}
}

#pragma endregion )

void ATagPlayerController::SetCurrentEffectHUD(const FString& EffectText)
{
	TagHUD = TagHUD == nullptr ? Cast<ATagHUD>(GetHUD()) : TagHUD;
	if (TagHUD && TagHUD->CharacterOverlay && TagHUD->CharacterOverlay->EffectText)
	{
		TagHUD->CharacterOverlay->EffectText->SetText(FText::FromString(EffectText));
	}
}

void ATagPlayerController::SetHUDTimerText(const float Time)
{
	TagHUD = TagHUD == nullptr ? Cast<ATagHUD>(GetHUD()) : TagHUD;
	if (TagHUD && TagHUD->CharacterOverlay && TagHUD->CharacterOverlay->GameTimer && TagHUD->CharacterOverlay->GameTimer->TimerText)
	{
		const int32 Minutes = FMath::FloorToInt(Time/60);
		const int32 Seconds = Time - (Minutes*60);
		const FString TimerText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		TagHUD->CharacterOverlay->GameTimer->TimerText->SetText(FText::FromString(TimerText));
	}
}

