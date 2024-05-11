// Fill out your copyright notice in the Description page of Project Settings.


#include "MapSelector.h"

#include "Components/ComboBoxString.h"
#include "Kismet/KismetSystemLibrary.h"


FString UMapSelector::GetSelectedMapURL()
{
	return BaseMapsPath + MapComboBox->GetSelectedOption();
}

void UMapSelector::NativeConstruct()
{
	Super::NativeConstruct();

	if (MapComboBox)
	{
		if (GetWorld())
		{
			MapComboBox->SetIsEnabled(UKismetSystemLibrary::IsServer(GetWorld()));
		}
	}

	for (FString MapName : MapNames)
	{
		MapComboBox->AddOption(MapName);
	}

	MapComboBox->SetSelectedIndex(0);
}
