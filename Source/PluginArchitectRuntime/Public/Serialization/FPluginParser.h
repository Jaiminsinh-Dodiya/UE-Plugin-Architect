#pragma once

#include "CoreMinimal.h"
#include "Descriptor/PluginArchitectDescriptor.h"

class FJsonObject;

class FPluginParser
{
public:
    static bool ToDescriptor(
        const TSharedPtr<FJsonObject>& JsonObject,
        FPluginArchitectDescriptor& OutDescriptor,
        FString& OutError
    );
};
