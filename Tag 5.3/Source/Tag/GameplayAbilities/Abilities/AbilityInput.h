#pragma once

UENUM(BlueprintType)
enum class EAbilityInput : uint8
{
	None = 0 UMETA(Hidden),
	Tag = 1,
	Jump = 2,
	Crouch = 3,
	Sprint = 4,
	Slide = 5,
	Dash = 6,
};