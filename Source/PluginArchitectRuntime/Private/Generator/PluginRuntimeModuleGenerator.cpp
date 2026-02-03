#include "Generator/PluginRuntimeModuleGenerator.h"
#include "Descriptor/PluginArchitectDescriptor.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

bool FPluginRuntimeModuleGenerator::P_M_Generate(
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
