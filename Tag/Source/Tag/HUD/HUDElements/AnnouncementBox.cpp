// Fill out your copyright notice in the Description page of Project Settings.


#include "AnnouncementBox.h"

#include "TagAnnouncement.h"
#include "Components/VerticalBox.h"

void UAnnouncementBox::AddAnnouncement(const FString& TaggerName, const FString& TaggedName)
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
