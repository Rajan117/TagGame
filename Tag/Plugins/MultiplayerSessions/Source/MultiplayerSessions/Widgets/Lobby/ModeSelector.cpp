// Fill out your copyright notice in the Description page of Project Settings.


#include "ModeSelector.h"
#include "Components/ComboBoxString.h"
#include "Kismet/KismetSystemLibrary.h"

void UModeSelector::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (ModeComboBox)
	{
		if (GetWorld())
		{
			ModeComboBox->SetIsEnabled(UKismetSystemLibrary::IsServer(GetWorld()));
		}
	}

	for (const TPair<FString, FString>& Pair : ModeNamesAndURLs)
	{
		ModeComboBox->AddOption(Pair.Key);
	}

	ModeComboBox->SetSelectedIndex(0);
}

FString UModeSelector::GetSelectedModeURL() const
{
	return *ModeNamesAndURLs.Find(ModeComboBox->GetSelectedOption());
}
