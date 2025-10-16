#include "TagTargetFilter.h"

#include "Tag/Character/TagCharacter.h"

bool FTagTargetFilter::FilterPassesForActor(const AActor* ActorToBeFiltered) const
{
	if (const bool SuperValue = Super::FilterPassesForActor(ActorToBeFiltered))
	{
		if (const ATagCharacter* TagCharacter = Cast<ATagCharacter>(ActorToBeFiltered))
		{
			return (bReverseFilter ^ !TagCharacter->GetIsTagged());
		}
	}
	return false;
}
