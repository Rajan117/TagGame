// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiplayerFunctionalTesting.h"

#if WITH_EDITOR
#include "Editor.h"
#include "FunctionalTestingModule.h"
#include "Settings/LevelEditorPlaySettings.h"
#include "Kismet/GameplayStatics.h"
#include "MultiplayerTest.h"
#endif

#define LOCTEXT_NAMESPACE "FMultiplayerFunctionalTestingModule"

void FMultiplayerFunctionalTestingModule::StartupModule()
{
#if WITH_EDITOR

	FEditorDelegates::PreBeginPIE.AddLambda([this](bool bSimulating)
		{
			AMultiplayerTest* Test = Cast<AMultiplayerTest>(UGameplayStatics::GetActorOfClass(GWorld, AMultiplayerTest::StaticClass()));
			if (Test)
			{
				const ETestPlayerCount TestPlayerCount = Test->GetTestPlayerCount();
				uint8 NumClients = GetCount(TestPlayerCount);

				const FRequestPlaySessionParams PlaySessionRequest = GEditor->GetPlaySessionRequest().GetValue();
				PlaySessionRequest.EditorPlaySettings->SetPlayNumberOfClients(NumClients);
				GEditor->RequestPlaySession(PlaySessionRequest);
			}
		});

#endif
}

void FMultiplayerFunctionalTestingModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMultiplayerFunctionalTestingModule, MultiplayerFunctionalTesting)