// Fill out your copyright notice in the Description page of Project Settings.


#include "TagAnimInstance.h"

#include "Tag/Character/TagCharacter.h"

void UTagAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	TagCharacter = Cast<ATagCharacter>(TryGetPawnOwner());
}

void UTagAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	TagCharacter = TagCharacter == nullptr ? Cast<ATagCharacter>(TryGetPawnOwner()) : TagCharacter;
	if (!TagCharacter) return;

	CharVelocity = TagCharacter->GetVelocity();
	
}
