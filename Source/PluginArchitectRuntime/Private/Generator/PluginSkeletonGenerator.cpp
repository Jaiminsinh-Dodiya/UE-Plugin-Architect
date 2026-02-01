#include "Generator/PluginSkeletonGenerator.h"

#include "Descriptor/PluginArchitectDescriptor.h"      // YOUR struct
#include "Misc/Paths.h"
#include "Logging/LogMacros.h"

bool FPluginSkeletonGenerator::Generate(
	const FPluginArchitectDescriptor& Descriptor,
	const FString& TargetPluginsDir,
	FString& OutError
)
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

	// ---- For v0.1: NO filesystem writes ----
	// This is only validation & planning

	return true;
}
