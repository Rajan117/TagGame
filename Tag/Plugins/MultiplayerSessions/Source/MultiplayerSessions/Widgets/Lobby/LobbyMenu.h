// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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

	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);

	void LoadMap();
};
