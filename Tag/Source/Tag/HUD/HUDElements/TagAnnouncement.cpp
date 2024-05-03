// Fill out your copyright notice in the Description page of Project Settings.


#include "TagAnnouncement.h"

#include "Components/TextBlock.h"

void UTagAnnouncement::SetTagAnnouncement(FString TaggerName, FString TaggedName)
{
	TaggingPlayerText->SetText(FText::FromString(TaggerName));
	TaggedPlayerText->SetText(FText::FromString(TaggedName));
}

void UTagAnnouncement::NativeConstruct()
{
	Super::NativeConstruct();

	GetWorld()->GetTimerManager().SetTimer(
	  AnnouncementTimerHandle,
	  this,
	  &UTagAnnouncement::RemoveFromParent,
	  AnnouncementTime,
	  true
	);
}
