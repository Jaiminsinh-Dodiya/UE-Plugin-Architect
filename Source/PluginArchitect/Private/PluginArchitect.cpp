#include "PluginArchitect.h"
#include "PluginArchitectMacros.h"
#include "HAL/IConsoleManager.h"
#include "Descriptor/PluginArchitectDescriptor.h"
#include "Generator/PluginSkeletonGenerator.h"
#include "Misc/Paths.h"

#define LOCTEXT_NAMESPACE "FPluginArchitectModule"

static FAutoConsoleCommand CreatePluginCommand(
    TEXT("PluginArchitect.CreatePlugin"),
    TEXT("Creates a runtime-only Unreal plugin. Usage: PluginArchitect.CreatePlugin <PluginName>"),
    FConsoleCommandWithArgsDelegate::CreateStatic(
        [](const TArray<FString> &Args)
        {
            if (Args.Num() < 1)
            {
                PA_LOG(Error, TEXT("Insufficient arguments provided for CreatePlugin command"));
                return;
            }

            const FString &PluginName = Args[0];

            // --------------------
            // Build descriptor (TEMP values for v0.1)
            // --------------------

            FPluginArchitectDescriptor Descriptor;
            Descriptor.FriendlyName = PluginName;
            Descriptor.Version = TEXT("1.0");
            Descriptor.FileVersion = 3;

            // One runtime module for now
            Descriptor.AddModule(
                PluginName + TEXT("Runtime"),
                EPluginModuleType::Runtime);

            // --------------------
            // Call runtime generator
            // --------------------
            FString Error;
            const FString PluginsDir = FPaths::ProjectPluginsDir();

            if (!FPluginSkeletonGenerator::Generate(Descriptor, PluginsDir, Error))
            {
                PA_LOG(Error, TEXT("Plugin generation failed: %s"), *Error);
            }
            else
            {
                PA_LOG(Log, TEXT("Plugin generation validated successfully for '%s'"), *PluginName);
            }
        }));

void FPluginArchitectModule::StartupModule()
{
    PA_LOG(Log, TEXT("PluginArchitect module started"));
}

void FPluginArchitectModule::ShutdownModule()
{
    PA_LOG(Log, TEXT("PluginArchitect module shutting down"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FPluginArchitectModule, PluginArchitect)