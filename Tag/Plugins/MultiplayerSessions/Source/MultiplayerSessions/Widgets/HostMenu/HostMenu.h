// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HostMenu.generated.h"

class UEditableTextBox;
class UCheckBox;
class UMultiplayerMainMenu;
class UMultiplayerSessionsSubsystem;
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

public:
	UHostMenu(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> LoadingWidgetClass;
	void ShowLoadingWidget();
	
private:
	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;
	UPROPERTY(meta = (BindWidget))
	UButton* HostButton;
	UPROPERTY(meta = (BindWidget))
	USlider* PlayerSlider;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CountText;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* InviteOnlyToggle;
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* PasswordTextBox;

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

	UPROPERTY(EditDefaultsOnly)
	uint8 DefaultPlayerCount;

	UPROPERTY()
	UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	UFUNCTION()
	void OnCreateSession(bool bWasSuccessful);
};
