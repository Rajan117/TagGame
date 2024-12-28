// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerBrowser.generated.h"

class UImage;
class UComboBoxString;
class UComboBox;
class UModeSelector;
class UMapSelector;
class UMultiplayerSessionsSubsystem;
class UServerListRow;
class UMultiplayerMainMenu;
class UButton;
class UTextBlock;
class UCircularThrobber;
class UScrollBox;
/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UServerBrowser : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(EditAnywhere)
	UTexture2D* SearchIcon;
	UPROPERTY(EditAnywhere)
	UTexture2D* RefreshIcon;

private:
	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;
	UPROPERTY(meta = (BindWidget))
	UButton* FindButton;
	UPROPERTY(meta = (BindWidget))
	UImage* FindButtonImage;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SearchStatusText;
	UPROPERTY(meta = (BindWidget))
	UCircularThrobber* LoadSymbol;
	UPROPERTY(meta = (BindWidget))
	UScrollBox* BrowserBox;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ModeFilter;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* MapFilter;

	UFUNCTION()
	void BackButtonClicked();
	UFUNCTION()
	void FindButtonClicked();
	
	UPROPERTY()
	UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;
	void OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);
	FDelegateHandle OnFindSessionsCompeteDelegateHandle;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UServerListRow> RowClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMultiplayerMainMenu> MenuClass;

	bool isEnabled;

	void StartSearch();
	void EndSearch();
	void Search();

	bool FilterResult(const FOnlineSessionSearchResult& SessionSearchResult); //Returns true if result is valid
};
