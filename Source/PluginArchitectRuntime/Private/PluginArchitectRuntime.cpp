#include "PluginArchitectRuntime.h"
#include "Modules/ModuleManager.h"

class FPluginArchitectRuntimeModule : public IModuleInterface
{
public:
    virtual void StartupModule() override
    {
        // EMPTY
    }

    virtual void ShutdownModule() override
    {
    }
};

IMPLEMENT_MODULE(FPluginArchitectRuntimeModule, PluginArchitectRuntime)
