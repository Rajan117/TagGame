// Fill out your copyright notice in the Description page of Project Settings.


#include "TagPlayerController.h"



#include "Tag/Character/TagCharacter.h"
#include "Tag/HUD/CharacterOverlay.h"
#include "Tag/HUD/TagHUD.h"
#include "Tag/HUD/HUDElements/GameTimer.h"
#include "Tag/GameModes/TagGameMode.h"
#include "Tag/HUD/Scoreboard/Scoreboard.h"
#include "Tag/GameStates/TagGameState.h"
#include "Tag/HUD/HUDElements/AnnouncementBox.h"
#include "Tag/HUD/HUDElements/MatchEndScreen.h"
#include "Tag/PlayerState/TagPlayerState.h"

#include "EnhancedInputComponent.h"
#include "Components/TextBlock.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

void ATagPlayerController::BeginPlay()
{
	Super::BeginPlay();

	const FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);
	SetShowMouseCursor(false);

	TagHUD = TagHUD == nullptr ? Cast<ATagHUD>(GetHUD()) : TagHUD;
	TagGameState = TagGameState == nullptr ? Cast<ATagGameState>(GetWorld()->GetGameState()) : TagGameState;
	if (TagGameState)
	{
		TagGameState->OnMatchStateChangedDelegate.AddDynamic(this, &ATagPlayerController::OnMatchStateSet);
	}
}

void ATagPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ATagPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();
	RefreshMatchInfo();
}

void ATagPlayerController::OnMatchStateSet(const FName State)
{
	MatchState = State;
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

void ATagPlayerController::RefreshMatchInfo()
{
	if (TagGameState)
	{
		MatchState = TagGameState->GetMatchState();
		MatchTime = TagGameState->MatchTime;
		WarmupTime = TagGameState->WarmupTime;
		RestartTime = TagGameState->RestartTime;
		LevelStartingTime = TagGameState->LevelStartingTime;
		RoundStartingTime = TagGameState->RoundStartingTime;
	}
}

void ATagPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);

	if (ATagCharacter* TagCharacter = Cast<ATagCharacter>(P))
	{
		TagCharacter->GetAbilitySystemComponent()->InitAbilityActorInfo(TagCharacter, TagCharacter);
	}

	RefreshMatchInfo();
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

#pragma region HUD

void ATagPlayerController::SetScoreTextHUD(const float Score)
{
	TagHUD = TagHUD == nullptr ? Cast<ATagHUD>(GetHUD()) : TagHUD;
	if (TagHUD && TagHUD->CharacterOverlay && TagHUD->CharacterOverlay->ScoreText)
	{
		const int32 Minutes = FMath::FloorToInt(Score/60);
		const int32 Seconds = Score - (Minutes*60);
		const FString TimerText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		TagHUD->CharacterOverlay->ScoreText->SetText(FText::FromString(TimerText));
	}
}

void ATagPlayerController::AddHUDTagAnnouncement(const FString& Tagger, const FString& Tagged)
{
	TagHUD = TagHUD == nullptr ? Cast<ATagHUD>(GetHUD()) : TagHUD;
	if (TagHUD && TagHUD->CharacterOverlay && TagHUD->CharacterOverlay->AnnouncementBox)
	{
		TagHUD->CharacterOverlay->AnnouncementBox->AddAnnouncement(Tagger, Tagged);
	}
}

#pragma endregion 

