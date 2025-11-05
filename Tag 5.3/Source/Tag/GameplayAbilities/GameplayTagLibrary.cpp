// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayTagLibrary.h"

// States
const FGameplayTag UGameplayTagLibrary::TaggedStateTag = FGameplayTag::RequestGameplayTag(FName("State.Tagged"));
const FGameplayTag UGameplayTagLibrary::CouldTagSomeoneStateTag = FGameplayTag::RequestGameplayTag(FName("State.CouldTagSomeone"));

// Effects
const FGameplayTag UGameplayTagLibrary::TaggedEffectTag = FGameplayTag::RequestGameplayTag(FName("Effect.Tagged"));

// Gameplay Cues
const FGameplayTag UGameplayTagLibrary::TaggedGameplayCueTag = FGameplayTag::RequestGameplayTag(FName("GameplayCue.Tagged"));

// Events
const FGameplayTag UGameplayTagLibrary::ChooseTaggerEventTag = FGameplayTag::RequestGameplayTag(FName("Event.ChooseTagger"));
const FGameplayTag UGameplayTagLibrary::TagEventTag = FGameplayTag::RequestGameplayTag(FName("Event.Tag"));
const FGameplayTag UGameplayTagLibrary::TagReceivedEventTag = FGameplayTag::RequestGameplayTag(FName("Event.TagReceived"));
const FGameplayTag UGameplayTagLibrary::TagGivenEventTag = FGameplayTag::RequestGameplayTag(FName("Event.TagGiven"));
