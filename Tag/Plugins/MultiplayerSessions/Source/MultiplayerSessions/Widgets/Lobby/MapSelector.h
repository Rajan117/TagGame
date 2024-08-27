// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapSelector.generated.h"

class FOnlineSessionSettings;
class FNamedOnlineSession;
class UComboBoxString;
/**
 * 
 */
UCLASS

()
class MULTIPLAYERSESSIONS_API UMapSelector : public UUserWidget
{
	GENERATED_BODY()
public:
	FString GetSelectedMapURL() const;

	UPROPERTY(EditDefaultsOnly)
	TArray<FString> MapNames;
	UPROPERTY(EditDefaultsOnly)
	FString BaseMapsPath = "";

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* MapComboBox;

	UFUNCTION()
	void OnSelectedMapChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	void OnSessionSettingsUpdated(FName SessionName, const FOnlineSessionSettings& UpdatedSettings);
	
	FNamedOnlineSession* CurrentSession;
	
};
