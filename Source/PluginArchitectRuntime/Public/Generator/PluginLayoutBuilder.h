#pragma once

#include "CoreMinimal.h"

/**
 * Responsible ONLY for creating plugin directory structure on disk.
 * No validation. No UI. No file generation.
 */
class PLUGINARCHITECTRUNTIME_API FPluginLayoutBuilder
{
public:
	/**
	 * Creates the directory layout for a plugin.
	 *
	 * @param PluginRootPath     Full path to plugin root (Plugins/TestPlugin)
	 * @param RuntimeModuleName  Name of the runtime module
	 * @param OutError           Error message on failure
	 *
	 * @return true if directories were created successfully
	 */
	static bool CreateLayout(
		const FString& PluginRootPath,
		const FString& RuntimeModuleName,
		FString& OutError
	);
};
