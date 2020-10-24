// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Omega : ModuleRules
{
	public Omega(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "HTTP", "Json", "JsonUtilities" });

        //PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
    }


}
