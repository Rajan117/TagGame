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
	virtual void LoadSetting() override;
	virtual void SaveSetting() override;
	virtual void ResetSetting() override;
	void Setup(FName InActionName, const FKeyMappingRow& InRowPair, UEnhancedInputUserSettings* InUserSettings);
	
private:
	UFUNCTION()
	void ResetButtonPressed();

	void SaveKeyMapping(FKey NewKey, EPlayerMappableKeySlot KeySlot);
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ActionNameText;
	UPROPERTY(meta = (BindWidget))
	UInputKeySelector* KeySlot1Selector;
	UPROPERTY(meta = (BindWidget))
	UInputKeySelector* KeySlot2Selector;
	UPROPERTY(meta = (BindWidget))
	UButton* ResetButton;
	
	UPROPERTY()
	UEnhancedInputUserSettings* UserSettings;
	FName ActionName;
	FKeyMappingRow RowPair;
	
};
