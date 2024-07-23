// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AnnouncementBox.generated.h"

class UPlayerEliminatedAnnouncement;
class ATagPlayerState;
class ATagGameState;
class ATagPlayerController;
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
	virtual void NativeConstruct() override;
	
	void AddAnnouncement(const FString& TaggerName, const FString& TaggedName) const;
	void AddEliminationAnnouncement(const FString& PlayerName) const;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* AnnouncementsBox;
	
protected:
	UFUNCTION()
	void OnPlayerTagged(ATagPlayerState* TaggingPlayer, ATagPlayerState* TaggedPlayer);
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UTagAnnouncement> AnnouncementClass;
	UFUNCTION()
	void OnPlayerEliminated(ATagPlayerState* EliminatedPlayer);
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerEliminatedAnnouncement> EliminationAnnouncementClass;

private:
	UPROPERTY()
	ATagGameState* TagGameState;
};
