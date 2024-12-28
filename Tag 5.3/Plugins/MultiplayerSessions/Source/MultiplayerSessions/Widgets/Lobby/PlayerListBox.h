// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerListBox.generated.h"

class UButton;
class ULobbyPlayerRow;
class UVerticalBox;
class UMultiplayerSessionsSubsystem;
/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UPlayerListBox : public UUserWidget
{
	GENERATED_BODY()
public:
	void Refresh();
	
protected:
	virtual void NativeConstruct() override;

	void OnSessionParticipantsChanged(FName SessionName, const FUniqueNetId& UniqueId, bool bJoined);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ULobbyPlayerRow> PlayerRowClass;

private:
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* PlayerBox;

	UPROPERTY(meta = (BindWidget))
	UButton* RefreshButton;
	
	UPROPERTY()
	UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	UFUNCTION()
	void RefreshButtonPressed();

	FTimerHandle RefreshTimerHandle;
};
