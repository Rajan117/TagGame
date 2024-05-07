// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerBrowser.generated.h"

class UMultiplayerSessionsSubsystem;
class UServerListRow;
class UMultiplayerMainMenu;
/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UServerBrowser : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* BackButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* FindButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* FindText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SearchStatusText;

	UPROPERTY(meta = (BindWidget))
	class UCircularThrobber* LoadSymbol;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* BrowserBox;

	UFUNCTION()
	void BackButtonClicked();

	UFUNCTION()
	void FindButtonClicked();
	
	UPROPERTY()
	UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	void OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UServerListRow> RowClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMultiplayerMainMenu> MenuClass;

	bool isEnabled;

	void StartSearch();
	void EndSearch();
};
