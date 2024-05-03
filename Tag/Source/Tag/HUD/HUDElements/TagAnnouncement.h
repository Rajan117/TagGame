// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TagAnnouncement.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class TAG_API UTagAnnouncement : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetTagAnnouncement(FString TaggerName, FString TaggedName);
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TaggingPlayerText;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TaggedPlayerText;

	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	float AnnouncementTime = 5.f;

private:
	FTimerHandle AnnouncementTimerHandle;
};
