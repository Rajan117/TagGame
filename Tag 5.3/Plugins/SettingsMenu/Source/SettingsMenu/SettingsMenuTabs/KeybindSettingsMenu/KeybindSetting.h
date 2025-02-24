// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	void ClearPrimaryBindingButtonPressed();
	UFUNCTION()
	void ClearSecondaryBindingButtonPressed();

	void SaveKeyMapping(FKey NewKey, EPlayerMappableKeySlot KeySlot);
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ActionNameText;
	UPROPERTY(meta = (BindWidget))
	UInputKeySelector* PrimaryKeySelector;
	UPROPERTY(meta = (BindWidget))
	UButton* ClearPrimaryBindingButton;
	UPROPERTY(meta = (BindWidget))
	UInputKeySelector* SecondaryKeySelector;
	UPROPERTY(meta = (BindWidget))
	UButton* ClearSecondaryBindingButton;
	
	UPROPERTY()
	UEnhancedInputUserSettings* UserSettings;
	FName ActionName;
	FKeyMappingRow RowPair;
	FPlayerKeyMapping PrimaryMapping;
	FPlayerKeyMapping SecondaryMapping;
	
};
