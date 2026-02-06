#pragma once

#include "CoreMinimal.h"

// Forward declaration – comes from YOUR Runtime/Public
struct FPluginArchitectDescriptor;

/**
 * Runtime-only plugin skeleton generator.
 * Uses FPluginArchitectDescriptor as the single source of truth.
 */
class PLUGINARCHITECTRUNTIME_API FPluginGenerator
{
public:
	/**
	 * Validates and prepares plugin generation.
	 *
	 * @param Descriptor        Plugin description data
	 * @param TargetPluginsDir  Base Plugins directory (e.g. Project/Plugins)
	 * @param OutError          Filled if generation cannot proceed
	 *
	 * @return true if generator can continue, false otherwise
	 */
	static bool GeneratePlugin(
		const FPluginArchitectDescriptor& Descriptor,
		const FString& TargetPluginsDir,
		FString& OutError
	);
	static bool CreateModule(
		const FPluginArchitectDescriptor& Descriptor,
		const FString& TargetPluginsDir,
		FString& OutError
	);
};