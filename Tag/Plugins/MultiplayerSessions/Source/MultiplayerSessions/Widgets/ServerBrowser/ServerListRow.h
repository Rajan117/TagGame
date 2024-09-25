// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "ServerListRow.generated.h"


class UServerPasswordEntry;
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

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> LoadingWidgetClass;
	void ShowLoadingWidget();

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

	FOnlineSessionSearchResult SearchResult;
	FString Password;
	UPROPERTY()
	UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;
	UPROPERTY()
	UServerBrowser* OwningBrowser;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UServerPasswordEntry> PasswordEntryWidgetClass;

	void OnJoinSession(EOnJoinSessionCompleteResult::Type Result);
	UFUNCTION()
	void JoinButtonClicked();
	UFUNCTION()
	void OnServerPasswordSubmitted(FString SubmittedPassword);
	
};
