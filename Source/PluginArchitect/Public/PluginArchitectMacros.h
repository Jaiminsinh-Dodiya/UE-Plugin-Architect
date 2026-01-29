#pragma once

#include "CoreMinimal.h"
#include "PluginArchitectLog.h"

#define PA_LOG(Verbosity, Format, ...) \
    UE_LOG(LogPluginArchitect, Verbosity, Format, ##__VA_ARGS__)