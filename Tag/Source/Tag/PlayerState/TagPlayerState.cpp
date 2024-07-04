// Fill out your copyright notice in the Description page of Project Settings.


#include "TagPlayerState.h"

//ReSharper disable once CppUnusedIncludeDirective
#include "Kismet/KismetSystemLibrary.h"

#include "Tag/Character/TagCharacter.h"
#include "Tag/Controller/TagPlayerController.h"

ATagPlayerState::ATagPlayerState()
{
	TagCharacter = TagCharacter == nullptr ? Cast<ATagCharacter>(GetPawn()) : TagCharacter;
	if (TagCharacter)
	{
		TagPlayerController = TagPlayerController == nullptr ? Cast<ATagPlayerController>(TagCharacter->Controller) : TagPlayerController;
	}
}

void ATagPlayerState::OnRep_Score()
{
	Super::OnRep_Score();

	TagCharacter = TagCharacter == nullptr ? Cast<ATagCharacter>(GetPawn()) : TagCharacter;
	if (TagCharacter)
	{
		TagPlayerController = TagPlayerController == nullptr ? Cast<ATagPlayerController>(TagCharacter->Controller) : TagPlayerController;
		if (TagPlayerController)
		{
			TagPlayerController->SetScoreTextHUD(GetScore());
		}
	}
	ScoreUpdatedDelegate.ExecuteIfBound(GetScore());
}

void ATagPlayerState::ServerSetScore(float ScoreAmount)
{
	if (!HasAuthority()) return;
	SetScore(ScoreAmount);
	
	TagCharacter = TagCharacter == nullptr ? Cast<ATagCharacter>(GetPawn()) : TagCharacter;
	if (TagCharacter)
	{
		TagPlayerController = TagPlayerController == nullptr ? Cast<ATagPlayerController>(TagCharacter->Controller) : TagPlayerController;
		if (TagPlayerController)
		{
			TagPlayerController->SetScoreTextHUD(GetScore());
		}
	}
	ScoreUpdatedDelegate.ExecuteIfBound(GetScore());
}
