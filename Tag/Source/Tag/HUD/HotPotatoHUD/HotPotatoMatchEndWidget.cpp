// Fill out your copyright notice in the Description page of Project Settings.


#include "HotPotatoMatchEndWidget.h"

#include "Components/TextBlock.h"
#include "Tag/Character/TagCharacter.h"
#include "Tag/Controller/TagPlayerController.h"

void UHotPotatoMatchEndWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (const ATagPlayerController* TagPlayerController = Cast<ATagPlayerController>(GetOwningPlayer()))
	{
		if (const ATagCharacter* TagCharacter = Cast<ATagCharacter>(TagPlayerController->GetCharacter()))
		{
			if (!TagCharacter->GetIsTagged())
			{
				MatchEndText->SetText(FText::FromString("You Won!"));
				return;
			}
		}
	}
	MatchEndText->SetText(FText::FromString("You Were Eliminated"));
}
