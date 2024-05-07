// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HostMenu.generated.h"

class UMultiplayerMainMenu;
class UButton;
class USlider;
class UTextBlock;
/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UHostMenu : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;

	UPROPERTY(meta = (BindWidget))
	UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	USlider* PlayerSlider;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CountText;

	UFUNCTION()
	void BackButtonClicked();

	UFUNCTION()
	void HostButtonClicked();

	UFUNCTION()
	void PlayerSliderChanged(float Value);

	int MaxPlayerCount;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMultiplayerMainMenu> MenuClass;

	UPROPERTY(EditDefaultsOnly)
	FString LobbyMapAddress;

	UPROPERTY()
	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	UFUNCTION()
	void OnCreateSession(bool bWasSuccessful);
};
