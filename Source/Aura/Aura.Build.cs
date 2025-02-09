using UnrealBuildTool;

public class Aura : ModuleRules
{
	public Aura(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] {"Aura"});

		PublicDependencyModuleNames.AddRange(new string[] {"Core", "CoreUObject", "Engine", "InputCore"});

		// Enhanced Input
		PrivateDependencyModuleNames.AddRange(new string[] {"EnhancedInput"});

		// Gameplay Ability System
		PrivateDependencyModuleNames.AddRange(new string[] {"GameplayAbilities", "GameplayTasks", "GameplayTags"});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}