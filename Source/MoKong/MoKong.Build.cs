// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MoKong : ModuleRules
{
	public MoKong(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// Public으로 된 모듈은 다른 모듈에서도 사용할 수 있음
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"AbilitySystem",
			"GameplayAbilities",
			"GameplayTasks",
			"GameplayTags",
			"FootSFX",
			"Locomotion",
			"SkillSystem",
			"Slate",
			"SlateCore",
			"UMG",
			"MotionWarping",
			"AdvancedThirdPersonCamera",
			"UISystem",
			"InventorySystem"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}