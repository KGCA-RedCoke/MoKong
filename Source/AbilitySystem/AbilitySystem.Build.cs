using UnrealBuildTool;

public class AbilitySystem : ModuleRules
{
	public AbilitySystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
				"UMG",
				"Slate",
				"SlateCore",
				"GameplayAbilities"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"GameplayTasks",
				"GameplayTags",
				"EnhancedInput",
				"CommonUI",
				"ModelViewViewModel",
				"DidItHit",
				"MotionWarping",
				"Niagara"
			}
		);
	}
}