// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionDelegates.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "LobbyMenu.generated.h"

class UModeSelector;
class ULobbyPlayerRow;
class UHostMenu;
class UButton;
class UVerticalBox;
class UPlayerListBox;
class UMapSelector;
/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API ULobbyMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	UPlayerListBox* PlayerListBox;

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> LoadingWidgetClass;
	void ShowLoadingWidget();
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;
	
private:
	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;
	UPROPERTY(meta = (BindWidget))
	UButton* StartButton;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* FriendBox;
	UPROPERTY(meta = (BindWidget))
	UMapSelector* MapSelector;
	UPROPERTY(meta = (BindWidget))
	UModeSelector* ModeSelector;
	
	UFUNCTION()
	void BackButtonClicked();
	UFUNCTION()
	void StartButtonClicked();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHostMenu> HostMenuClass;

	UPROPERTY(EditDefaultsOnly)
	FString StartMapAddress = FString("Game/Maps/GameStartMap");

	UPROPERTY()
	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;
	IOnlineSessionPtr SessionInterface;

	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);
	UFUNCTION()
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	void LoadMap();
};
