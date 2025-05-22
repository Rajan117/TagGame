#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/Actor.h"
#include "Abilities/GameplayAbilityTargetDataFilter.h"
#include "TagTargetFilter.generated.h"

USTRUCT(BlueprintType)
struct TAG_API FTagTargetFilter : public FGameplayTargetDataFilter
{
	GENERATED_USTRUCT_BODY()
	
	/** Returns true if the actor passes the filter and will be targeted */
	virtual bool FilterPassesForActor(const AActor* ActorToBeFiltered) const override;
};
