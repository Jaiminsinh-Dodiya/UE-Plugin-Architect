#include "Generator/PluginModuleGenerator.h"
#include "Descriptor/PluginArchitectDescriptor.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

bool FPluginModuleGenerator::GenerateModule(
	const FPluginArchitectDescriptor& Descriptor,
	const FString& PluginRootDir,
	FString& OutError)
{
	const FString ModuleName = Descriptor.FriendlyName + TEXT("Runtime");

	const FString ModuleDir =
		FPaths::Combine(PluginRootDir, TEXT("Source"), ModuleName);

	const FString PublicDir = FPaths::Combine(ModuleDir, TEXT("Public"));
	const FString PrivateDir = FPaths::Combine(ModuleDir, TEXT("Private"));

	// ---- Create directories ----
	if (!IFileManager::Get().MakeDirectory(*PublicDir, true) ||
		!IFileManager::Get().MakeDirectory(*PrivateDir, true))
	{
		OutError = TEXT("Failed to create runtime module directories");
		return false;
	}

	// ---- Header (.h) ----
	const FString HeaderPath =
		FPaths::Combine(PublicDir, ModuleName + TEXT(".h"));

	const FString HeaderContent = FString::Printf(
		TEXT(
			"#pragma once\n\n"
			"#include \"Modules/ModuleManager.h\"\n\n"
			"class F%sModule : public IModuleInterface\n"
			"{\n"
			"public:\n"
			"\tvirtual void StartupModule() override;\n"
			"\tvirtual void ShutdownModule() override;\n"
			"};\n"
		),
		*ModuleName
	);

	if (!FFileHelper::SaveStringToFile(HeaderContent, *HeaderPath))
	{
		OutError = TEXT("Failed to write runtime module header");
		return false;
	}

	// ---- Source (.cpp) ----
	const FString SourcePath =
		FPaths::Combine(PrivateDir, ModuleName + TEXT(".cpp"));

	const FString SourceContent = FString::Printf(
		TEXT(
			"#include \"%s.h\"\n\n"
			"#define LOCTEXT_NAMESPACE \"%s\"\n\n"
			"void F%sModule::StartupModule() {}\n\n"
			"void F%sModule::ShutdownModule() {}\n\n"
			"#undef LOCTEXT_NAMESPACE\n\n"
			"IMPLEMENT_MODULE(F%sModule, %s)\n"
		),
		*ModuleName,
		*ModuleName,
		*ModuleName,
		*ModuleName,
		*ModuleName,
		*ModuleName
	);

	if (!FFileHelper::SaveStringToFile(SourceContent, *SourcePath))
	{
		OutError = TEXT("Failed to write runtime module source");
		return false;
	}

	return true;
}


bool FPluginModuleGenerator::GenerateBuildCsContent(
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
