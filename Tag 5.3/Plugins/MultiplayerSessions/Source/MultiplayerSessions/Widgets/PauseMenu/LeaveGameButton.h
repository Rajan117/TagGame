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
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> LoadingWidgetClass;
	
	UPROPERTY(EditDefaultsOnly)
	FString MainMenuMapAddress = FString("/Game/Tag/Maps/MainMenu");
	
	void ShowLoadingWidget();
	
	UFUNCTION()
	void OnLeaveButtonClicked();
	
	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);

private:
	UPROPERTY()
	UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;
};
