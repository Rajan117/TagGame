// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "ModeSelector.generated.h"

class FOnlineSessionSettings;
class FNamedOnlineSession;
class UComboBoxString;
/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UModeSelector : public UUserWidget
{
	GENERATED_BODY()
public:
	FString GetSelectedModeURL() const;
	FString GetSelectedMode() const;
	
	UPROPERTY(EditDefaultsOnly)
	TMap<FString, FString> ModeNamesAndURLs; //Maps mode name to its URL

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ModeComboBox;

	UFUNCTION()
	void OnSelectedModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	UFUNCTION()
	void OnSessionSettingsChanged();

	FNamedOnlineSession* CurrentSession;
	IOnlineSessionPtr SessionInterface;
};
