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

	TArray<FKey> GetSelectedKeys() const;
	bool IsKeySelected();
	FName GetActionName() const { return ActionName; }
	
private:
	UFUNCTION()
	void ResetButtonPressed();

	UFUNCTION()
	void OnKeySlot1Selected(FInputChord SelectedKey);
	UFUNCTION()
	void OnKeySlot2Selected(FInputChord SelectedKey);

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
	FPlayerKeyMapping Slot1Mapping;
	FPlayerKeyMapping Slot2Mapping;
	bool bHas2Slots = false;
	
};
