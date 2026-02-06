#pragma once

#include "CoreMinimal.h"
#include "Descriptor/PluginModuleDescriptor.h"

class FPluginModuleTypeUtils
{
public:

    static bool FromString(const FString& InString, EPluginModuleType& OutType)
    {
        if (InString.Equals(TEXT("Runtime"), ESearchCase::IgnoreCase))
        {
            OutType = EPluginModuleType::Runtime;
            return true;
        }

        if (InString.Equals(TEXT("Editor"), ESearchCase::IgnoreCase))
        {
            OutType = EPluginModuleType::Editor;
            return true;
        }

        if (InString.Equals(TEXT("Developer"), ESearchCase::IgnoreCase))
        {
            OutType = EPluginModuleType::Developer;
            return true;
        }

        if (InString.Equals(TEXT("Engine"), ESearchCase::IgnoreCase))
        {
            OutType = EPluginModuleType::Engine;
            return true;
        }

        return false;
    }
};
