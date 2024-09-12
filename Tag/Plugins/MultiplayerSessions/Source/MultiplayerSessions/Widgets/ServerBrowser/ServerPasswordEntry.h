// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerPasswordEntry.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPasswordSubmitted, FString, SubmittedPassword);

class UButton;
class UEditableTextBox;
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

	FOnPasswordSubmitted OnPasswordSubmittedDelegate;

protected:
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* PasswordTextBox;
	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;
	UPROPERTY(meta = (BindWidget))
	UButton* CancelButton;

private:
	void OnJoinButtonClicked();
	void OnCancelButtonClicked();
	
};
