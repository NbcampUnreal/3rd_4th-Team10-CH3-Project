using UnrealBuildTool;

public class Team10_Project : ModuleRules
{
	public Team10_Project(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput", 
			"GameplayAbilities", 
			"GameplayTags", 
			"GameplayTasks",
            "UMG",
            "AIModule",
			"Niagara"
        });

		PrivateDependencyModuleNames.AddRange(new string[] {  });


        PublicIncludePaths.AddRange(new string[] {
			"ProjectPracGAS",
			"ProjectPracGAS/Public"
			});
    }
}
