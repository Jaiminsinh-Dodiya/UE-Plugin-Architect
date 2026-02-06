#pragma once

#include "CoreMinimal.h"
#include "Descriptor/PluginArchitectDescriptor.h"

class PLUGINARCHITECTRUNTIME_API FPluginModifier
{
public:

    static bool AddModule(
        const FString& PluginName,
        const FString& ModuleName,
        EPluginModuleType Type,
        FString& OutError
    );
};