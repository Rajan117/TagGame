// Fill out your copyright notice in the Description page of Project Settings.


#include "ResolutionScaleGraphicSetting.h"

#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"

void UResolutionScaleGraphicSetting::NativeConstruct()
{
	Super::NativeConstruct();
	ResolutionScaleSlider->OnValueChanged.AddDynamic(this, &UResolutionScaleGraphicSetting::OnResolutionScaleValueChanged);
}

void UResolutionScaleGraphicSetting::LoadSetting()
{
	if (!UserSettings) return;
	const float ResolutionScale = UserSettings->GetResolutionScaleNormalized();
	ResolutionScaleSlider->SetValue(ResolutionScale);
	ResolutionScaleText->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), ResolutionScale)));
}

void UResolutionScaleGraphicSetting::SaveSetting()
{
	const float ResolutionScale = ResolutionScaleSlider->GetValue();
	UserSettings->SetResolutionScaleValueEx(ResolutionScaleSlider->GetValue()*100);
	UserSettings->ApplySettings(false);
	UserSettings->ApplyResolutionSettings(false);
}

void UResolutionScaleGraphicSetting::OnResolutionScaleValueChanged(float Value)
{
	ResolutionScaleText->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), Value*100)));
}
