// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyMenu.generated.h"

class ULobbyPlayerRow;
class UHostMenu;
class UButton;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API ULobbyMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	void Refresh();

protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;

	UPROPERTY(meta = (BindWidget))
	UButton* StartButton;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* PlayerBox;

	UPROPERTY(meta = (BindWidget))
	UButton* RefreshButton;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* FriendBox;

	UFUNCTION()
	void BackButtonClicked();

	UFUNCTION()
	void StartButtonClicked();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ULobbyPlayerRow> PlayerRowClass;

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