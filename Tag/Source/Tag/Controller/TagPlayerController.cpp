// Fill out your copyright notice in the Description page of Project Settings.


#include "TagPlayerController.h"


#include "Tag/Character/TagCharacter.h"
#include "Tag/HUD/CharacterOverlay.h"
#include "Tag/HUD/TagHUD.h"
#include "Tag/HUD/HUDElements/GameTimer.h"
#include "Tag/HUD/HUDElements/GameStartTimer.h"

#include "Components/TextBlock.h"
#include "GameFramework/GameMode.h"
#include "Net/UnrealNetwork.h"

void ATagPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TagHUD = TagHUD == nullptr ? Cast<ATagHUD>(GetHUD()) : TagHUD;
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

void ATagPlayerController::OnMatchStateSet(FName State)
{
	MatchState = State;

	if (MatchState == MatchState::InProgress)
	{
		TagHUD = TagHUD == nullptr ? Cast<ATagHUD>(GetHUD()) : TagHUD;
		if (TagHUD) TagHUD->AddCharacterOverlay();
		StartGameStartCountdown();
	}
}

void ATagPlayerController::OnRep_MatchState()
{
	if (MatchState == MatchState::InProgress)
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
			GameStartTimer->AddToViewport();
		}
	}
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
	const uint32 SecondsLeft = FMath::CeilToInt(MatchTime-GetServerTime());
	if (TimerInt != SecondsLeft)
	{
		SetHUDTimerText(MatchTime-GetServerTime());
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

