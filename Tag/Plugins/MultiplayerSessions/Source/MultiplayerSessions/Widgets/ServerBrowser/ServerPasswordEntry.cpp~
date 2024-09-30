// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerPasswordEntry.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"

void UServerPasswordEntry::NativeConstruct()
{
	Super::NativeConstruct();

	if (JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this, &UServerPasswordEntry::OnJoinButtonClicked);
	}
	
	if (CancelButton)
	{
		CancelButton->OnClicked.AddDynamic(this, &UServerPasswordEntry::OnCancelButtonClicked);
	}
}

void UServerPasswordEntry::NativeDestruct()
{
	OnPasswordSubmittedDelegate.Clear();
	Super::NativeDestruct();
}

void UServerPasswordEntry::SpawnInit(FString SessionPassword)
{
	Password = SessionPassword;
}

void UServerPasswordEntry::OnJoinButtonClicked()
{
	const FString SubmittedPassword = PasswordTextBox->GetText().ToString();
	if (SubmittedPassword == Password)
	{
		PasswordStatusText->SetText(FText::FromString("Joining..."));
		OnPasswordSubmittedDelegate.Broadcast(SubmittedPassword);
		RemoveFromParent();
	}
	else
	{
		PasswordStatusText->SetText(FText::FromString("Incorrect Password"));
	}
}

void UServerPasswordEntry::OnCancelButtonClicked()
{
	RemoveFromParent();
}
