// Copyright Epic Games, Inc. All Rights Reserved.

#include "TagGameplayTags.h"

#include "Engine/EngineTypes.h"
#include "GameplayTagsManager.h"

namespace TagGameplayTags
{
	// Movement Abilities
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Dash, "Ability.Dash", "Tag for the dash ability.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Jump, "Ability.Jump", "Tag for the jump ability.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Slide, "Ability.Slide", "Tag for the slide ability.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Crouch, "Ability.Crouch", "Tag for the crouch ability.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Sprint, "Ability.Sprint", "Tag for the sprint ability.");

	// Tagging Abilities
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_TagPassive, "Ability.TagPassive", "Tag for passive tagging-related ability/state.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Tag, "Ability.Tag", "Primary tag ability used to mark a player as tagged.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Tag_Cooldown, "Ability.Tag.Cooldown", "Cooldown tag applied while the tag ability is on cooldown.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Tag_Disabled, "Ability.Tag.Disabled", "Tag indicating tagging is currently disabled for the character.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_SelfTag, "Ability.SelfTag", "Tag used when a player self-tags.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ApplyTag, "Ability.ApplyTag", "Internal tag used when applying the tagged state to a character.");
	
	// Gameplay Effects
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Effect_SpeedBuff, "Effect.SpeedBuff", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Effect_StaminaRegen, "Effect.StaminaRegen", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Effect_TagCooldown, "Effect.TagCooldown", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Effect_Tagged, "Effect.Tagged", "Tag for Tagged Gameplay Effect");

	// Gameplay Cues
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayCue_Hero_Sprint, "GameplayCue.Hero.Sprint", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayCue_Tagged, "GameplayCue.Tagged", "Cosmetic FX that occur whilst tagged.");

	// States
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_CouldTagSomeone, "State.CouldTagSomeone", "State when player is in tagging range of a non-tagged person.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Crouching, "State.Crouching", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Dead, "State.Dead", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Jumping, "State.Jumping", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Sliding, "State.Sliding", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Sprinting, "State.Sprinting", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Tagged, "State.Tagged", "The state when a player is tagged.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Untagged, "State.Untagged", "The state when a player is not tagged.");

	// Events
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_ChooseTagger, "Event.ChooseTagger", "Event where a tagger is chosen.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Tag, "Event.Tag", "Event where a tag has occurred.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_TagReceived, "Event.TagReceived", "Event where the player receiving this event was tagged.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_TagGiven, "Event.TagGiven", "Event where the player receiving this event tagged someone else.");
	
}
