#pragma once

UENUM(BlueprintType)
enum class EAbilityInput : uint8
{
	None = 0 UMETA(Hidden),
	Tag = 1,
	Jump = 2,
	Crouch = 3,
};
