// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MultiplayerFunctionalTesting : ModuleRules
{
	public MultiplayerFunctionalTesting(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
                // ... add public include paths required here ...
            }
            );
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
                // ... add other private include paths required here ...
            }
            );
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"UMG"
            }
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "EnhancedInput",
                "FunctionalTesting"
            }
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

#if UE_5_3_OR_LATER
        bool bIsBuildingForEditor = Target.bCompileAgainstEditor;
#else
		bool bIsBuildingForEditor = Target.bBuildEditor;
#endif

        if (bIsBuildingForEditor && Target.Configuration != UnrealTargetConfiguration.Shipping)
        {
            PrivateDependencyModuleNames.Add("UnrealEd");
        }
    }
}
