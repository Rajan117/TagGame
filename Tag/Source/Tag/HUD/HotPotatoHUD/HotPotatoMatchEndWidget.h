// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tag/HUD/HUDElements/MatchEndScreen.h"
#include "HotPotatoMatchEndWidget.generated.h"

/**
 * 
 */
UCLASS()
class TAG_API UHotPotatoMatchEndWidget : public UMatchEndScreen
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MatchEndText;
};
