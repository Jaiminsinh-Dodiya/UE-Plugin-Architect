#include "Generator/PluginRuntimeBuildCsGenerator.h"
#include "Descriptor/PluginArchitectDescriptor.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

bool FPluginRuntimeBuildCsGenerator::GenerateBuildCsContent(
	const FPluginArchitectDescriptor& Descriptor,
	const FString& PluginRootDir,
	FString& OutError)
{
	const FString ModuleName = Descriptor.FriendlyName + TEXT("Runtime");

	const FString BuildCsPath =
		FPaths::Combine(
			PluginRootDir,
			TEXT("Source"),
			ModuleName,
			ModuleName + TEXT(".Build.cs")
		);

	const FString BuildCsContent = FString::Printf(
		TEXT(
			"using UnrealBuildTool;\n\n"
			"public class %s : ModuleRules\n"
			"{\n"
			"\tpublic %s(ReadOnlyTargetRules Target) : base(Target)\n"
			"\t{\n"
			"\t\tPCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;\n\n"
			"\t\tPublicDependencyModuleNames.AddRange(new string[]\n"
			"\t\t{\n"
			"\t\t\t\"Core\",\n"
			"\t\t\t\"CoreUObject\",\n"
			"\t\t\t\"Engine\"\n"
			"\t\t});\n"
			"\t}\n"
			"}\n"
		),
		*ModuleName,
		*ModuleName
	);

	if (!FFileHelper::SaveStringToFile(BuildCsContent, *BuildCsPath))
	{
		OutError = TEXT("Failed to write runtime Build.cs");
		return false;
	}

	return true;
}
