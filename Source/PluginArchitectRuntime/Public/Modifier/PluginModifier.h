#pragma once

#include "CoreMinimal.h"
#include "Descriptor/PluginModuleDescriptor.h"

class PLUGINARCHITECTRUNTIME_API FPluginModifier
{
public:

static bool GenerateModule(
    const FString& PluginName,
    const FString& ModuleName,
    EPluginModuleType Type,
    FString& OutError
);

};
