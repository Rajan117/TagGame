// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerPasswordEntry.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"

void UServerPasswordEntry::NativeConstruct()
{
	Super::NativeConstruct();

	if (JoinButton) JoinButton->OnClicked.AddDynamic(this, &UServerPasswordEntry::OnJoinButtonClicked);
	if (CancelButton) CancelButton->OnClicked.AddDynamic(this, &UServerPasswordEntry::OnCancelButtonClicked);
}

void UServerPasswordEntry::NativeDestruct()
{
	OnPasswordSubmittedDelegate.Clear();
	Super::NativeDestruct();
}

void UServerPasswordEntry::OnJoinButtonClicked()
{
	OnPasswordSubmittedDelegate.Broadcast(PasswordTextBox->GetText().ToString());
	RemoveFromParent();
}

void UServerPasswordEntry::OnCancelButtonClicked()
{
	RemoveFromParent();
}
