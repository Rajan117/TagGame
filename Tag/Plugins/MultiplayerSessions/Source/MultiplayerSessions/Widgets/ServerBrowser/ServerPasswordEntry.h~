// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerPasswordEntry.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPasswordSubmitted, FString, SubmittedPassword);

class UButton;
class UEditableTextBox;
class UTextBlock;
/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UServerPasswordEntry : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	void SpawnInit(FString SessionPassword);
	FOnPasswordSubmitted OnPasswordSubmittedDelegate;

protected:
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* PasswordTextBox;
	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;
	UPROPERTY(meta = (BindWidget))
	UButton* CancelButton;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PasswordStatusText;

private:
	UFUNCTION()
	void OnJoinButtonClicked();
	UFUNCTION()
	void OnCancelButtonClicked();

	FString Password;
	
};
