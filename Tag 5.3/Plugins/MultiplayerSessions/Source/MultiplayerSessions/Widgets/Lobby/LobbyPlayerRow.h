// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyPlayerRow.generated.h"

class UHostMenu;
class UButton;
class UVerticalBox;
class UTextBlock;
/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API ULobbyPlayerRow : public UUserWidget
{
	GENERATED_BODY()
public:
	void SpawnInitialize(APlayerState* Ref);

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* NameText;

	UPROPERTY(meta = (BindWidget))
	UButton* KickButton;

	UFUNCTION()
	void KickButtonClicked();

	UPROPERTY()
	APlayerState* PlayerStateRef;
	
};
