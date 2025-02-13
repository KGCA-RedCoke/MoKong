using UnrealBuildTool;

public class UISystem : ModuleRules
{
	public UISystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"UMG",
				"Slate",
				"SlateCore",
				"GameplayAbilities"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"CommonUI",
				"ModelViewViewModel"
			}
		);
	}
}