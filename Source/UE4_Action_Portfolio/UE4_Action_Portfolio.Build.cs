// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UE4_Action_Portfolio : ModuleRules
{
	public UE4_Action_Portfolio(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore",
		"NavigationSystem", "AIModule", "GameplayTasks","LevelSequence","MovieScene"});

		PublicIncludePaths.Add(ModuleDirectory);
	}
}
