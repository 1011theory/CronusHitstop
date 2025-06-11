// Copyright The First Ones, 2024. All Rights Reserved.

using UnrealBuildTool;

public class CronusHitstop : ModuleRules
{
	public CronusHitstop(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "Engine"
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				//"Engine",
				"Slate",
				"SlateCore",
                "Niagara"
			}
			);
	}
}
