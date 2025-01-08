//

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Templates/SubclassOf.h"

#include "ClientWorldFunctionLibrary.generated.h"

UCLASS()
class MULTIPLAYERFUNCTIONALTESTING_API UClientWorldFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Gets a world with specified index. ClientIndex=0 means to return the first world found in engine's FWorldContext array, not necessarily the one for client index 0. */
	static UWorld* GetClientWorld(int32 ClientIndex);

	/** Same as UE's GetAllActorsOfClass except works in client net mode functional tests since it'll query the client's world. Also works in standalone net mode. */
	UFUNCTION(BlueprintCallable, Category = "Multiplayer Functional Testing", meta = (DeterminesOutputType = "ActorClass", DynamicOutputParam = "OutActors"))
	static void GetAllClientActorsOfClass(TSubclassOf<AActor> ActorClass, TArray<AActor*>& OutActors, int32 ClientIndex = 0);

	/** Same as UE's GetAllActorsOfClassWithTag except works in client net mode functional tests since it'll query the client's world. Also works in standalone net mode. */
	UFUNCTION(BlueprintCallable, Category = "Multiplayer Functional Testing", meta = (DeterminesOutputType = "ActorClass", DynamicOutputParam = "OutActors"))
	static void GetAllClientActorsOfClassWithTag(TSubclassOf<AActor> ActorClass, FName Tag, TArray<AActor*>& OutActors, int32 ClientIndex = 0);

	/** Same as UE's GetActorOfClass except can filter by tag and works in client net mode functional tests since it'll query the client's world. Also works in standalone net mode. */
	UFUNCTION(BlueprintCallable, Category = "Multiplayer Functional Testing", meta = (DeterminesOutputType = "ActorClass"))
	static AActor* GetClientActorOfClassWithTag(TSubclassOf<AActor> ActorClass, FName Tag, int32 ClientIndex = 0);

	/** Same as UE's GetActorOfClass except works in client net mode functional tests since it'll query the client's world. Also works in standalone net mode. */
	UFUNCTION(BlueprintCallable, Category = "Multiplayer Functional Testing", meta = (DeterminesOutputType = "ActorClass"))
	static AActor* GetClientActorOfClass(TSubclassOf<AActor> ActorClass, int32 ClientIndex = 0);

	/** Same as UE's GetAllWidgetsOfClass except works in client net mode functional tests since it'll query the client's world. Also works in standalone net mode. */
	UFUNCTION(BlueprintCallable, Category = "Multiplayer Functional Testing", meta = (DeterminesOutputType = "WidgetClass", DynamicOutputParam = "FoundWidgets"))
	static void GetAllClientWidgetsOfClass(TArray<UUserWidget*>& FoundWidgets, TSubclassOf<UUserWidget> WidgetClass, bool TopLevelOnly = true, int32 ClientIndex = 0);
};
