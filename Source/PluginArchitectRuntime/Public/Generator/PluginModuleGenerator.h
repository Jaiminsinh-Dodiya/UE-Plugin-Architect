#pragma once

#include "CoreMinimal.h"
#include "Descriptor/PluginArchitectDescriptor.h"

/**
 * Generates runtime module code for a plugin based on its descriptor.
 */
class FPluginModuleGenerator
{
public:
    /**
     * Generates the runtime module code.
     *
     * @param Descriptor The plugin descriptor.
     * @param TargetPluginsDir The target directory for the plugin.
     * @param OutError Error message on failure.
     * @return true if generation succeeded.
     */
    static bool GenerateModule(
        const FPluginArchitectDescriptor &Descriptor,
        const FString &PluginRootDir,
        FString &OutError);

    static bool GenerateBuildCsContent(
        const FPluginArchitectDescriptor &PluginDescriptor,
        const FString &OutBuildCsContent,
        FString &OutError);
};