#pragma once

#include "CoreMinimal.h"

// Forward declaration – comes from YOUR Runtime/Public
struct FPluginArchitectDescriptor;

/**
 * Generates the Build.cs file for the runtime module of a plugin.
 */
class FPluginRuntimeBuildCsGenerator
{
    public:
    /**
     * Generates the content for a Build.cs file based on the provided plugin descriptor.
     * @param PluginDescriptor The descriptor containing plugin configuration details.
     * @param OutBuildCsContent The generated Build.cs content as a string.
     * @param OutError An output string to capture any error messages during generation.
     */

     static bool GenerateBuildCsContent(
        const FPluginArchitectDescriptor& PluginDescriptor, 
        const FString& OutBuildCsContent,
        FString& OutError
    );
};