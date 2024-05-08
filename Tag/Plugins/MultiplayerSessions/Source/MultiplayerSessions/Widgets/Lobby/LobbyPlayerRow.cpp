// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerRow.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

void ULobbyPlayerRow::SpawnInitialize(APlayerState* Ref)
{
	PlayerStateRef = Ref;
}

void ULobbyPlayerRow::NativeConstruct()
{
	Super::NativeConstruct();

	if (KickButton)
	{
		if (GetWorld())
		{
			if (const APlayerController* Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0))
			{
				const bool Value = UKismetSystemLibrary::IsServer(GetWorld()) && Controller->PlayerState != PlayerStateRef;
				KickButton->SetIsEnabled(Value);
			}
		}
		KickButton->OnClicked.AddDynamic(this, &ULobbyPlayerRow::KickButtonClicked);
	}

	if (NameText && PlayerStateRef)
	{
		NameText->SetText(FText::FromString(PlayerStateRef->GetPlayerName()));
	}
}

void ULobbyPlayerRow::KickButtonClicked()
{
}
