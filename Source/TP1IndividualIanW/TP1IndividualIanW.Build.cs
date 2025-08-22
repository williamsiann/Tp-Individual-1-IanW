// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TP1IndividualIanW : ModuleRules
{
	public TP1IndividualIanW(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"TP1IndividualIanW",
			"TP1IndividualIanW/Variant_Platforming",
			"TP1IndividualIanW/Variant_Platforming/Animation",
			"TP1IndividualIanW/Variant_Combat",
			"TP1IndividualIanW/Variant_Combat/AI",
			"TP1IndividualIanW/Variant_Combat/Animation",
			"TP1IndividualIanW/Variant_Combat/Gameplay",
			"TP1IndividualIanW/Variant_Combat/Interfaces",
			"TP1IndividualIanW/Variant_Combat/UI",
			"TP1IndividualIanW/Variant_SideScrolling",
			"TP1IndividualIanW/Variant_SideScrolling/AI",
			"TP1IndividualIanW/Variant_SideScrolling/Gameplay",
			"TP1IndividualIanW/Variant_SideScrolling/Interfaces",
			"TP1IndividualIanW/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
