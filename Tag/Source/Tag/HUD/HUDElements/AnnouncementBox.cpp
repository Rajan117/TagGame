// Fill out your copyright notice in the Description page of Project Settings.


#include "AnnouncementBox.h"

#include "GameTimer.h"
#include "TagAnnouncement.h"
#include "Components/VerticalBox.h"
#include "Tag/Controller/TagPlayerController.h"
#include "Tag/GameStates/TagGameState.h"
#include "Tag/PlayerState/TagPlayerState.h"

void UAnnouncementBox::NativeConstruct()
{
	Super::NativeConstruct();
	
	TagGameState = Cast<ATagGameState>(GetWorld()->GetGameState());
	if (TagGameState)
	{
		TagGameState->OnPlayerTaggedDelegate.AddDynamic(this, &UAnnouncementBox::OnPlayerTagged);
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

void UAnnouncementBox::OnPlayerTagged(ATagPlayerState* TaggingPlayer, ATagPlayerState* TaggedPlayer)
{
	AddAnnouncement(TaggingPlayer->GetPlayerName(), TaggedPlayer->GetPlayerName());
}
