// Fill out your copyright notice in the Description page of Project Settings.


#include "AnnouncementBox.h"

#include "GameTimer.h"
#include "TagAnnouncement.h"
#include "Tag/HUD/HotPotatoHUD/PlayerEliminatedAnnouncement.h"
#include "Components/VerticalBox.h"
#include "Tag/GameStates/TagGameState.h"
#include "Tag/GameStates/TagRoundBasedGameState.h"
#include "Tag/PlayerState/TagPlayerState.h"

void UAnnouncementBox::NativeConstruct()
{
	Super::NativeConstruct();
	
	TagGameState = Cast<ATagGameState>(GetWorld()->GetGameState());
	if (TagGameState)
	{
		TagGameState->OnPlayerTaggedDelegate.AddDynamic(this, &UAnnouncementBox::OnPlayerTagged);
		TagRoundBasedGameState = Cast<ATagRoundBasedGameState>(TagGameState);
		if (TagRoundBasedGameState)
		{
			TagRoundBasedGameState->OnPlayerEliminatedDelegate.AddDynamic(this, &UAnnouncementBox::OnPlayerEliminated);
		}
	}
}

void UAnnouncementBox::AddAnnouncement(const FString& TaggerName, const FString& TaggedName) const
{
	if (AnnouncementClass)
	{
		if (UTagAnnouncement* Announcement = CreateWidget<UTagAnnouncement>(GetWorld(), AnnouncementClass))
		{
			AnnouncementsBox->AddChild(Announcement);
			Announcement->SetTagAnnouncement(TaggerName, TaggedName);
		}
	}
}

void UAnnouncementBox::AddEliminationAnnouncement(const FString& PlayerName) const
{
	if (EliminationAnnouncementClass)
	{
		if (UPlayerEliminatedAnnouncement* Announcement = CreateWidget<UPlayerEliminatedAnnouncement>(GetWorld(), EliminationAnnouncementClass))
		{
			AnnouncementsBox->AddChild(Announcement);
			Announcement->SetEliminationAnnouncement(PlayerName);
		}
	}
}

void UAnnouncementBox::OnPlayerTagged(ATagPlayerState* TaggingPlayer, ATagPlayerState* TaggedPlayer)
{
	AddAnnouncement(TaggingPlayer->GetPlayerName(), TaggedPlayer->GetPlayerName());
}

void UAnnouncementBox::OnPlayerEliminated(ATagPlayerState* EliminatedPlayer)
{
	AddEliminationAnnouncement(EliminatedPlayer->GetPlayerName());
}
