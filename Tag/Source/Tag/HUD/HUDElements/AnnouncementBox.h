// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AnnouncementBox.generated.h"

class UTagAnnouncement;

class UVerticalBox;
/**
 * 
 */
UCLASS()
class TAG_API UAnnouncementBox : public UUserWidget
{
	GENERATED_BODY()
public:
	void AddAnnouncement(const FString& TaggerName, const FString& TaggedName);
	
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* AnnouncementsBox;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UTagAnnouncement> AnnouncementClass;
	
};
