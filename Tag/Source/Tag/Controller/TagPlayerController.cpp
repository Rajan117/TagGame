// Fill out your copyright notice in the Description page of Project Settings.


#include "TagPlayerController.h"


#include "EnhancedInputComponent.h"
#include "Tag/Character/TagCharacter.h"
#include "Tag/HUD/CharacterOverlay.h"
#include "Tag/HUD/TagHUD.h"
#include "Tag/HUD/HUDElements/GameTimer.h"
#include "Tag/HUD/HUDElements/GameStartTimer.h"
#include "Tag/HUD/HUDElements/MatchEndScreen.h"
#include "Tag/GameModes/TagGameMode.h"
#include "Tag/HUD/Scoreboard/Scoreboard.h"

#include "Components/TextBlock.h"
#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Tag/HUD/HUDElements/AnnouncementBox.h"
#include "Tag/HUD/HUDElements/MatchEndScreen.h"
#include "Tag/PlayerState/TagPlayerState.h"

void ATagPlayerController::BeginPlay()
{
	Super::BeginPlay();

	const FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);
	SetShowMouseCursor(false);

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
		HandleWarmup();
	}
	if (MatchState == MatchState::PostMatch)
	{
		HandlePostMatch();
	}
}

void ATagPlayerController::OnRep_MatchState()
{
	//ServerCheckMatchState();
	
	if (MatchState == MatchState::Warmup)
	{
		HandleWarmup();
	}
	if (MatchState == MatchState::PostMatch)
	{
		HandlePostMatch();
	}
}

void ATagPlayerController::HandleWarmup()
{
	if (!bInitialisedHUD)
	{
		TagHUD = TagHUD == nullptr ? Cast<ATagHUD>(GetHUD()) : TagHUD;
		if (TagHUD) TagHUD->AddCharacterOverlay();
		bInitialisedHUD = true;
	}
	StartGameStartCountdown();
}

void ATagPlayerController::HandlePostMatch()
{
	if (ATagCharacter* TagCharacter = Cast<ATagCharacter>(GetCharacter()))
	{
		TagCharacter->bShouldUpdateScore = false;
		TagCharacter->DisableInput(this);
	}

	if (MatchEndWidgetClass)
	{
		MatchEndWidgetRef = CreateWidget<UMatchEndScreen>(this, MatchEndWidgetClass);
		if (MatchEndWidgetRef)
		{
			MatchEndWidgetRef->AddToViewport();
			MatchEndWidgetRef->StartTimer(RestartTime);
		}
	}
	
	UKismetSystemLibrary::PrintString(this, "Match Ended");
	TagHUD = TagHUD == nullptr ? Cast<ATagHUD>(GetHUD()) : TagHUD;
	if (TagHUD) TagHUD->RemoveCharacterOverlay();
	HideScoreboard();
	ShowScoreboard();
}

void ATagPlayerController::ShowScoreboard()
{
	if (ScoreboardClass)
	{
		ScoreboardRef = CreateWidget<UScoreboard>(this, ScoreboardClass);
		if (ScoreboardRef)
		{
			ScoreboardRef->AddToViewport();
		}
	}
}

void ATagPlayerController::HideScoreboard()
{
	if (ScoreboardRef)
	{
		ScoreboardRef->RemoveFromParent();
	}
}

void ATagPlayerController::StartGameStartCountdown()
{
	if (GameStartTimerClass)
	{
		if (UGameStartTimer* GameStartTimer = CreateWidget<UGameStartTimer>(GetWorld(), GameStartTimerClass))
		{
			GameStartTimer->StartTimer(WarmupTime);
			GameStartTimer->AddToViewport();
		}
	}
}

void ATagPlayerController::ServerCheckMatchState_Implementation()
{
	if (const ATagGameMode* TagGameMode = Cast<ATagGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		WarmupTime = TagGameMode->WarmupTime;
		MatchTime = TagGameMode->MatchTime;
		LevelStartingTime = TagGameMode->LevelStartingTime;
		RoundStartingTime = TagGameMode->RoundStartingTime;
		MatchState = TagGameMode->GetMatchState();
		RestartTime = TagGameMode->RestartGameTime;
		ClientJoinMidgame(MatchState, WarmupTime, MatchTime, LevelStartingTime, RoundStartingTime, RestartTime);
	}
}

void ATagPlayerController::ClientJoinMidgame_Implementation(FName StateOfMatch, float Warmup, float Match,
	float LevelStart, float RoundStart, float Restart)
{
	WarmupTime = Warmup;
	MatchTime = Match;
	LevelStartingTime =  LevelStart;
	RoundStartingTime = RoundStart;
	RestartTime = Restart;
	MatchState = StateOfMatch;
	OnMatchStateSet(MatchState);
}

void ATagPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);

	if (ATagCharacter* TagCharacter = Cast<ATagCharacter>(P))
	{
		TagCharacter->GetAbilitySystemComponent()->InitAbilityActorInfo(TagCharacter, TagCharacter);
	}
}

void ATagPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (ScoreboardAction)
		{
			EnhancedInputComponent->BindAction(ScoreboardAction, ETriggerEvent::Started, this, &ThisClass::ShowScoreboard);
			EnhancedInputComponent->BindAction(ScoreboardAction, ETriggerEvent::Completed, this, &ThisClass::HideScoreboard);
		}
	}
}

void ATagPlayerController::BroadcastTag(ATagPlayerState* TaggingPLayer, ATagPlayerState* TaggedPlayer)
{
	ClientTagAnnouncement(TaggingPLayer, TaggedPlayer);
}

void ATagPlayerController::ClientTagAnnouncement_Implementation(ATagPlayerState* TaggingPlayer,
	ATagPlayerState* TaggedPlayer)
{
	AddHUDTagAnnouncement(TaggingPlayer->GetPlayerName(), TaggedPlayer->GetPlayerName());
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

#pragma region HUD

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

void ATagPlayerController::SetScoreTextHUD(const float Score)
{
	TagHUD = TagHUD == nullptr ? Cast<ATagHUD>(GetHUD()) : TagHUD;
	if (TagHUD && TagHUD->CharacterOverlay && TagHUD->CharacterOverlay->ScoreText)
	{
		const int32 Minutes = FMath::FloorToInt(Score/60);
		const int32 Seconds = Score - (Minutes*60);
		const FString TimerText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		TagHUD->CharacterOverlay->ScoreText->SetText(FText::FromString(FString::SanitizeFloat(Score)));
	}
}

void ATagPlayerController::AddHUDTagAnnouncement(FString Tagger, FString Tagged)
{
	TagHUD = TagHUD == nullptr ? Cast<ATagHUD>(GetHUD()) : TagHUD;
	if (TagHUD && TagHUD->CharacterOverlay && TagHUD->CharacterOverlay->AnnouncementBox)
	{
		TagHUD->CharacterOverlay->AnnouncementBox->AddAnnouncement(Tagger, Tagged);
	}
}

void ATagPlayerController::SetHUDTime()
{
	uint32 SecondsLeft = MatchTime;
	if (MatchState == MatchState::InMatch) SecondsLeft = FMath::CeilToInt(WarmupTime+MatchTime-GetServerTime()+RoundStartingTime+5);
	if (SecondsLeft <= 10)
	{
		TagHUD = TagHUD == nullptr ? Cast<ATagHUD>(GetHUD()) : TagHUD;
		if (TagHUD && TagHUD->CharacterOverlay && TagHUD->CharacterOverlay->GameTimer && TagHUD->CharacterOverlay->GameTimer->TimerText)
		{
			TagHUD->CharacterOverlay->GameTimer->TimerText->SetColorAndOpacity(FSlateColor(FLinearColor(1, 0.f, 0.f, 1)));
		}
	}
	SetHUDTimerText(SecondsLeft);
	
	return;
	if (TimerInt != SecondsLeft)
	{
		SetHUDTimerText(SecondsLeft);
	}
	TimerInt = SecondsLeft;
}

#pragma endregion 

