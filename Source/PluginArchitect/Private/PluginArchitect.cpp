#include "PluginArchitect.h"
#include "PluginArchitectMacros.h"
#include "HAL/IConsoleManager.h"

#define LOCTEXT_NAMESPACE "FPluginArchitectModule"

static FAutoConsoleCommand CreatePluginCommand(
    TEXT("PluginArchitect.CreatePlugin"),
    TEXT("Creates a runtime-only Unreal plugin. Usage: PluginArchitect.CreatePlugin <PluginName>"),
    FConsoleCommandWithArgsDelegate::CreateStatic(
        [](const TArray<FString>& Args)
        {
            if (Args.Num() < 1)
            {
                PA_LOG(Error, TEXT("Insufficient arguments provided for CreatePlugin command"));
                return;
            }

            const FString& PluginName = Args[0];
            PA_LOG(Log, TEXT("Plugin '%s' created successfully"), *PluginName);

            // Generator call will go here next
        }
    )
);

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
