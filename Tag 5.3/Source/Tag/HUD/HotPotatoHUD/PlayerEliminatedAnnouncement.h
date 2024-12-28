// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerEliminatedAnnouncement.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class TAG_API UPlayerEliminatedAnnouncement : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	void SetEliminationAnnouncement(FString EliminatedPlayerName);
	UPROPERTY(meta = (BindWidget))
	UTextBlock* EliminatedPlayerText;

protected:
	UPROPERTY(EditDefaultsOnly)
	float AnnouncementTime = 5.f;

private:
	FTimerHandle AnnouncementTimerHandle;
};
