// Fill out your copyright notice in the Description page of Project Settings.


#include "HostMenu.h"

#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/EditableTextBox.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MultiplayerSessions/Subsystems/MultiplayerSessionsSubsystem.h"
#include "MultiplayerSessions/Widgets/MultiplayerMainMenu.h"

UHostMenu::UHostMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DefaultPlayerCount = 4;
}

void UHostMenu::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UHostMenu::BackButtonClicked);
	}

	if (HostButton)
	{
		HostButton->OnClicked.AddDynamic(this, &UHostMenu::HostButtonClicked);
	}

	if (PlayerSlider)
	{
		PlayerSlider->SetMinValue(2.f);
		PlayerSlider->SetMaxValue(16.f);
		PlayerSlider->OnValueChanged.AddDynamic(this, &UHostMenu::PlayerSliderChanged);
		PlayerSlider->SetValue(DefaultPlayerCount);
		MaxPlayerCount = DefaultPlayerCount;
	}

	if (const UGameInstance* GameInstance = GetGameInstance())
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}

	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSession);
	}
}

void UHostMenu::ShowLoadingWidget()
{
	if (LoadingWidgetClass)
	{
		if (UUserWidget* LoadingWidgetRef = CreateWidget<UUserWidget>(this, LoadingWidgetClass))
		{
			LoadingWidgetRef->AddToViewport();
		}
	}
}

void UHostMenu::BackButtonClicked()
{
	if (MenuClass)
	{
		if (UMultiplayerMainMenu* Menu = CreateWidget<UMultiplayerMainMenu>(this, MenuClass))
		{
			if (BackButton) BackButton->SetIsEnabled(false);
			Menu->AddToViewport();
			Menu->MenuSetup();
			RemoveFromParent();
		}
	}
}

void UHostMenu::HostButtonClicked()
{
	if (HostButton)
	{
		HostButton->SetIsEnabled(false);
	}
	if (MultiplayerSessionsSubsystem)
	{
		ShowLoadingWidget();
		MultiplayerSessionsSubsystem->CreateSession(MaxPlayerCount, FString("Tag"), InviteOnlyToggle->IsChecked(), PasswordTextBox->GetText().ToString());
	}
}

void UHostMenu::PlayerSliderChanged(float Value)
{
	MaxPlayerCount = FMath::TruncToInt(FMath::Clamp(Value, 2.0f, 16.0f));
	CountText->SetText(FText::FromString(FString::FromInt(MaxPlayerCount)));
}

void UHostMenu::OnCreateSession(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		if (UWorld* World = GetWorld())
		{
			World->ServerTravel(LobbyMapAddress+ "?listen");
		}
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, "Failed To Create Session");
		HostButton->SetIsEnabled(true);
	}
}
