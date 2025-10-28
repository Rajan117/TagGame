// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayTagLibrary.h"

// States
const FGameplayTag UGameplayTagLibrary::TaggedStateTag = FGameplayTag::RequestGameplayTag(FName("State.Tagged"));
const FGameplayTag UGameplayTagLibrary::CouldTagSomeoneStateTag = FGameplayTag::RequestGameplayTag(FName("State.CouldTagSomeone"));

// Events
const FGameplayTag UGameplayTagLibrary::ChooseTaggerEventTag = FGameplayTag::RequestGameplayTag(FName("Event.ChooseTagger"));

// Effects
const FGameplayTag UGameplayTagLibrary::TaggedEffectTag = FGameplayTag::RequestGameplayTag(FName("Effect.Tagged"));

// Gameplay Cues
const FGameplayTag UGameplayTagLibrary::TaggedGameplayCueTag = FGameplayTag::RequestGameplayTag(FName("GameplayCue.Tagged"));
