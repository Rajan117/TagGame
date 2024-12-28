// Fill out your copyright notice in the Description page of Project Settings.


#include "ResolutionGraphicSetting.h"

#include "Components/ComboBoxString.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"

void UResolutionGraphicSetting::LoadSetting()
{
	if (!UserSettings) return;
	PopulateResolutionComboBox();
	const FIntPoint Resolution = UserSettings->GetScreenResolution();
	const FString ResolutionString = FString::Printf(TEXT("%dx%d"), Resolution.X, Resolution.Y);
	if (ResolutionComboBox->FindOptionIndex(ResolutionString) == -1) ResolutionComboBox->AddOption(ResolutionString);
	ResolutionComboBox->SetSelectedOption(ResolutionString);
}

void UResolutionGraphicSetting::SaveSetting()
{
	const FString SelectedOption = ResolutionComboBox->GetSelectedOption();
	FString XString;
	FString YString;
	if (SelectedOption.Split(TEXT("x"), &XString, &YString))
	{
		const int32 X = FCString::Atoi(*XString);
		const int32 Y = FCString::Atoi(*YString);
		UserSettings->SetScreenResolution(FIntPoint(X, Y));
		UserSettings->ApplySettings(false);
		UserSettings->ApplyResolutionSettings(false);
	}
}

void UResolutionGraphicSetting::PopulateResolutionComboBox()
{
	if (!UserSettings) return;

	ResolutionComboBox->ClearOptions();
	TArray<FIntPoint> Resolutions;
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(Resolutions);
	for (const FIntPoint& Resolution : Resolutions)
	{
		const FString ResolutionString = FString::Printf(TEXT("%dx%d"), Resolution.X, Resolution.Y);
		ResolutionComboBox->AddOption(ResolutionString);
	}
}
