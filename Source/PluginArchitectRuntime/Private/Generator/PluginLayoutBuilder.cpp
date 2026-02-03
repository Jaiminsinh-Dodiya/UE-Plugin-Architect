#include "Generator/PluginLayoutBuilder.h"

#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Logging/LogMacros.h"

bool FPluginLayoutBuilder::CreateLayout(
	const FString& PluginRootPath,
	const FString& RuntimeModuleName,
	FString& OutError
)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	// 1. Fail if plugin already exists
	if (PlatformFile.DirectoryExists(*PluginRootPath))
	{
		OutError = FString::Printf(
			TEXT("Plugin folder already exists: %s"),
			*PluginRootPath
		);
		UE_LOG(LogTemp, Error, TEXT("%s"), *OutError);
		return false;
	}

	UE_LOG(LogTemp, Log, TEXT("Creating plugin layout at: %s"), *PluginRootPath);

	// 2. Define directories
	const FString SourceDir   = FPaths::Combine(PluginRootPath, TEXT("Source"));
	const FString ModuleDir   = FPaths::Combine(SourceDir, RuntimeModuleName);
	const FString PublicDir   = FPaths::Combine(ModuleDir, TEXT("Public"));
	const FString PrivateDir  = FPaths::Combine(ModuleDir, TEXT("Private"));
	const FString ResourceDir = FPaths::Combine(PluginRootPath, TEXT("Resources"));

	// 3. Create directories
	if (!PlatformFile.CreateDirectoryTree(*PublicDir))
	{
		OutError = TEXT("Failed to create Public directory");
		return false;
	}

	if (!PlatformFile.CreateDirectoryTree(*PrivateDir))
	{
		OutError = TEXT("Failed to create Private directory");
		return false;
	}

	if (!PlatformFile.CreateDirectoryTree(*ResourceDir))
	{
		OutError = TEXT("Failed to create Resources directory");
		return false;
	}

	UE_LOG(LogTemp, Log, TEXT("Plugin directory layout created successfully"));
	return true;
}
