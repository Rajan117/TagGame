// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../../../../../../Source/Tag/Settings/TagPlayerMappableKeyProfile.h"
#include "SettingsMenu/SettingsMenuTabs/Setting.h"
#include "KeybindSetting.generated.h"

class UButton;
class UInputKeySelector;
class UTextBlock;
struct FKeyMappingRow;
class UEnhancedInputUserSettings;
/**
 * 
 */
UCLASS()
class SETTINGSMENU_API UKeybindSetting : public USetting
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void Init(UEnhancedInputUserSettings* InUserSettings, FName InActionName, FKeyMappingRow& InRowPair);
	virtual void LoadSetting() override;
	virtual void SaveSetting() override;
	virtual void ResetSetting() override;
	
private:
	UFUNCTION()
	void ResetButtonPressed();
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ActionNameText;
	UPROPERTY(meta = (BindWidget))
	UInputKeySelector* KeySelector;
	UPROPERTY(meta = (BindWidget))
	UButton* ResetButton;

	
	UPROPERTY()
	UEnhancedInputUserSettings* UserSettings;
	UPROPERTY()
	FName ActionName;
	UPROPERTY()
	FKeyMappingRow RowPair;
	
};
