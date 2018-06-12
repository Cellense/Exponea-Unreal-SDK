// Mr.Fox and friends, s.r.o, https://github.com/alexhajdu, https://twitter.com/alexhajdu

using UnrealBuildTool;

public
class InfinarioSDK : ModuleRules
{
public
	InfinarioSDK( ReadOnlyTargetRules Target )
		: base( Target )
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange( new string[]{
			"InfinarioSDK/Public"
			// ... add public include paths required here ...
		} );


		PrivateIncludePaths.AddRange( new string[]{
			"InfinarioSDK/Private",
			// ... add other private include paths required here ...
		} );


		PublicDependencyModuleNames.AddRange( new string[]{
			"Core",
			// ... add other public dependencies that you statically link with here ...
		} );


		PrivateDependencyModuleNames.AddRange( new string[]{
			"CoreUObject", "Engine", "Slate", "SlateCore", "Http", "Json", "JsonUtilities",
			// ... add private dependencies that you statically link with here ...
		} );


		DynamicallyLoadedModuleNames.AddRange( new string[]{
			// ... add any modules that your module loads dynamically here ...
		} );
	}
}
