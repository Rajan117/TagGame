#include "TagTargetFilter.h"

bool FTagTargetFilter::FilterPassesForActor(const AActor* ActorToBeFiltered) const
{
	if (const bool SuperValue = Super::FilterPassesForActor(ActorToBeFiltered))
	{
		const bool SpecificCondition = ActorToBeFiltered->GetClass()->IsChildOf(RequiredActorClass);
		return (bReverseFilter ^ SpecificCondition);
	}
	return false;
}
