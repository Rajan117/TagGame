// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LeaveGameButton.generated.h"

class UMultiplayerSessionsSubsystem;
class UButton;
/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API ULeaveGameButton : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	UPROPERTY(meta =(BindWidget))
	UButton* LeaveButton;
	
protected:
	UFUNCTION()
	void OnLeaveButtonClicked();
	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);

private:
	UPROPERTY(EditDefaultsOnly)
	FString MainMenuURL = FString("/Game/Tag/Maps/Lobby?game=/Game/Tag/Blueprints/GameModes/BP_TagLobbyGameMode.BP_TagLobbyGameMode_C");

	UPROPERTY()
	UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;
};
