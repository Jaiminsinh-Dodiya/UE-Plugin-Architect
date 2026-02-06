#include "Generator/PluginGenerator.h"

#include "Descriptor/PluginArchitectDescriptor.h"
#include "Generator/PluginLayoutBuilder.h"
#include "Misc/Paths.h"
#include "Logging/LogMacros.h"

//Serialization
#include "Serialization/PluginUPluginSerializer.h"
#include "HAL/FileManager.h"
#include "Misc/FileHelper.h"

// Generate Runtime .Build.cs and module code
#include "Generator/PluginModuleGenerator.h"


bool FPluginGenerator::Generate(
	const FPluginArchitectDescriptor &Descriptor,
	const FString &TargetPluginsDir,
	FString &OutError)
{
	UE_LOG(LogTemp, Log, TEXT("[PluginSkeletonGenerator] Generate called"));

	// ---- Validate plugin descriptor ----

	if (Descriptor.FriendlyName.IsEmpty())
	{
		OutError = TEXT("Plugin FriendlyName is empty");
		UE_LOG(LogTemp, Error, TEXT("%s"), *OutError);
		return false;
	}

	if (Descriptor.Modules.Num() == 0)
	{
		OutError = TEXT("Plugin must contain at least one module");
		UE_LOG(LogTemp, Error, TEXT("%s"), *OutError);
		return false;
	}

	// ---- Validate target directory ----

	if (TargetPluginsDir.IsEmpty())
	{
		OutError = TEXT("Target Plugins directory is empty");
		UE_LOG(LogTemp, Error, TEXT("%s"), *OutError);
		return false;
	}

	// Normalize path
	const FString PluginsDir = FPaths::ConvertRelativePathToFull(TargetPluginsDir);

	// Compute plugin root path
	const FString PluginRootDir = FPaths::Combine(PluginsDir, Descriptor.FriendlyName);

	UE_LOG(LogTemp, Log, TEXT("Plugin Name      : %s"), *Descriptor.FriendlyName);
	UE_LOG(LogTemp, Log, TEXT("Plugin Root Path : %s"), *PluginRootDir);
	UE_LOG(LogTemp, Log, TEXT("Module Count     : %d"), Descriptor.Modules.Num());

	// ---- Create plugin layout ----

	const FString RuntimeModuleName =
		Descriptor.FriendlyName + TEXT("Runtime");

	if (!FPluginLayoutBuilder::CreateLayout(
			PluginRootDir,
			RuntimeModuleName,
			OutError))
	{
		UE_LOG(LogTemp, Error, TEXT("Layout creation failed: %s"), *OutError);
		return false;
	}

	// ---- Serialize .uplugin file ----
	FString UPluginJson;
	if (!FPluginUPluginSerializer::Serialize(
			Descriptor,
			UPluginJson,
			OutError))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to serialize .uplugin: %s"), *OutError);
		return false;
	}

	// ---- Write .uplugin file ----
	const FString UPluginFilePath =
		FPaths::Combine(PluginRootDir, Descriptor.FriendlyName + TEXT(".uplugin"));

	if (!FFileHelper::SaveStringToFile(UPluginJson, *UPluginFilePath))
	{
		OutError = FString::Printf(TEXT("Failed to write .uplugin file: %s"), *UPluginFilePath);
		UE_LOG(LogTemp, Error, TEXT("%s"), *OutError);
		return false;
	}
	UE_LOG(LogTemp, Log, TEXT("Successfully wrote .uplugin file: %s"), *UPluginFilePath);

	// ---- Generate runtime module ----
	if (!FPluginModuleGenerator::GenerateModule(
			Descriptor,
			PluginRootDir,
			OutError))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to generate runtime module: %s"), *OutError);
		return false;
	}

	// ---- Generate runtime Build.cs ----
	if (!FPluginModuleGenerator::GenerateBuildCsContent(
			Descriptor,
			PluginRootDir,
			OutError))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to generate runtime Build.cs: %s"), *OutError);
		return false;
	}

	return true;
}