// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayTagLibrary.h"

const FGameplayTag UGameplayTagLibrary::TaggedStateTag = FGameplayTag::RequestGameplayTag(FName("State.Tagged"));
const FGameplayTag UGameplayTagLibrary::ChooseTaggerEventTag = FGameplayTag::RequestGameplayTag(FName("Event.ChooseTagger"));
const FGameplayTag UGameplayTagLibrary::TaggedEffectTag = FGameplayTag::RequestGameplayTag(FName("Effect.Tagged"));
