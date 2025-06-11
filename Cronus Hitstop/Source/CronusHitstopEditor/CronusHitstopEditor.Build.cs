// Copyright The First Ones, 2024. All Rights Reserved.

using UnrealBuildTool;

public class CronusHitstopEditor : ModuleRules
{
	public CronusHitstopEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		  
        PrivateDependencyModuleNames.AddRange(
        new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "UnrealEd",
                "Slate",
                "SlateCore",
                "Projects",
                "ClassViewer",
                "BlueprintGraph",
                "InputCore"
            }
        );
        PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"CronusHitstop",
				"AssetTools"
			}
			);
	}
}
