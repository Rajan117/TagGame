// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "ServerListRow.generated.h"


class UImage;
class UTextBlock;
class UButton;
class UMultiplayerSessionsSubsystem;
class UServerBrowser;
/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UServerListRow : public UUserWidget
{
	GENERATED_BODY()
public:
	void SpawnInitialize(FOnlineSessionSearchResult Result, UServerBrowser* Browser);

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MatchText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MapText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerCountText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PingText;

	UPROPERTY(meta = (BindWidget))
	UImage* PasswordImage;

	UFUNCTION()
	void JoinButtonClicked();

	FOnlineSessionSearchResult SearchResult;

	UPROPERTY()
	UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;
	UPROPERTY()
	UServerBrowser* OwningBrowser;

	void OnJoinSession(EOnJoinSessionCompleteResult::Type Result);

	FString Password;
	
};
