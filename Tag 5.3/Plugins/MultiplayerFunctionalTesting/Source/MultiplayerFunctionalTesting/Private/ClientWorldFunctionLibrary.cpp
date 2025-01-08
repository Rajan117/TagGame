//

#include "ClientWorldFunctionLibrary.h"

#include "Blueprint/UserWidget.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "UObject/UObjectIterator.h"

UWorld* UClientWorldFunctionLibrary::GetClientWorld(const int32 ClientIndex = 0)
{
	const TIndirectArray<FWorldContext>& WorldContexts = GEngine->GetWorldContexts();
	for (const FWorldContext& Context : WorldContexts)
	{
		if (Context.World() != nullptr)
		{
			if (Context.WorldType == EWorldType::PIE /*&& Context.PIEInstance == 0*/ || Context.WorldType == EWorldType::Game)
			{
				const ENetMode NetMode = Context.World()->GetNetMode();
				if (NetMode == NM_Client || NetMode == NM_Standalone)
				{
					if (ClientIndex > 0)
					{
						// Subtract -1 to match logic in SceneOutliner::GetWorldDescription
						if (Context.PIEInstance - 1 == ClientIndex)
						{
							return Context.World();
						}
					}
					else
					{
						return Context.World();
					}
				}
			}
		}
	}
	return nullptr;
}

void UClientWorldFunctionLibrary::GetAllClientActorsOfClass(const TSubclassOf<AActor> ActorClass, TArray<AActor*>& OutActors, const int32 ClientIndex)
{
	OutActors.Reset();

	// We do nothing if no is class provided, rather than giving ALL actors!
	if (ActorClass)
	{

		if (const UWorld* World = GetClientWorld(ClientIndex))
		{
			for (TActorIterator<AActor> It(World, ActorClass); It; ++It)
			{
				AActor* Actor = *It;
				OutActors.Add(Actor);
			}
		}
	}
}

void UClientWorldFunctionLibrary::GetAllClientActorsOfClassWithTag(const TSubclassOf<AActor> ActorClass, const FName Tag, TArray<AActor*>& OutActors, const int32 ClientIndex)
{
	OutActors.Reset();

	const UWorld* World = GetClientWorld(ClientIndex);

	// We do nothing if no is class provided, rather than giving ALL actors!
	if (ActorClass && World)
	{
		for (TActorIterator<AActor> It(World, ActorClass); It; ++It)
		{
			AActor* Actor = *It;
			if (IsValid(Actor) && Actor->ActorHasTag(Tag))
			{
				OutActors.Add(Actor);
			}
		}
	}
}

AActor* UClientWorldFunctionLibrary::GetClientActorOfClassWithTag(const TSubclassOf<AActor> ActorClass, const FName Tag, const int32 ClientIndex)
{
	// We do nothing if no is class provided
	if (ActorClass)
	{
		if (const UWorld* World = GetClientWorld(ClientIndex))
		{
			for (TActorIterator<AActor> It(World, ActorClass); It; ++It)
			{
				AActor* Actor = *It;
				if (IsValid(Actor) && Actor->ActorHasTag(Tag))
				{
					return Actor;
				}
			}
		}
	}

	return nullptr;
}

AActor* UClientWorldFunctionLibrary::GetClientActorOfClass(const TSubclassOf<AActor> ActorClass, const int32 ClientIndex)
{
	// We do nothing if no is class provided
	if (ActorClass)
	{
		if (const UWorld* World = GetClientWorld(ClientIndex))
		{
			for (TActorIterator<AActor> It(World, ActorClass); It; ++It)
			{
				AActor* Actor = *It;
				return Actor;
			}
		}
	}

	return nullptr;
}

FORCENOINLINE void UClientWorldFunctionLibrary::GetAllClientWidgetsOfClass(TArray<UUserWidget*>& FoundWidgets, const TSubclassOf<UUserWidget> WidgetClass, const bool TopLevelOnly, const int32 ClientIndex)
{
	// Prevent possibility of an ever-growing array if user uses this in a loop
	FoundWidgets.Empty();

	const UWorld* World = GetClientWorld(ClientIndex);

	if (!WidgetClass)
	{
		return;
	}

	if (!World)
	{
		return;
	}

	for (TObjectIterator<UUserWidget> Itr; Itr; ++Itr)
	{
		UUserWidget* LiveWidget = *Itr;

		// Skip any widget that's not in the current world context or that is not a child of the class specified.
		if (LiveWidget->GetWorld() != World || !LiveWidget->GetClass()->IsChildOf(WidgetClass))
		{
			continue;
		}

		if (TopLevelOnly)
		{
			if (LiveWidget->IsInViewport())
			{
				FoundWidgets.Add(LiveWidget);
			}
		}
		else
		{
			FoundWidgets.Add(LiveWidget);
		}
	}
}
