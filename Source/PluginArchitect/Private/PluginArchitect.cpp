#include "PluginArchitect.h"
#include "PluginArchitectMacros.h"
#include "HAL/IConsoleManager.h"
#include "Descriptor/PluginArchitectDescriptor.h"
#include "Generator/PluginGenerator.h"
#include "Modifier/PluginModifier.h"
#include "Misc/Paths.h"

#define LOCTEXT_NAMESPACE "FPluginArchitectModule"

////////////////////////////////////////////////////////////////////////////////////////////////////
// Helpers
////////////////////////////////////////////////////////////////////////////////////////////////////

static bool StringToModuleType(const FString& InType, EPluginModuleType& OutType)
{
    if (InType.Equals(TEXT("Runtime"), ESearchCase::IgnoreCase))
    {
        OutType = EPluginModuleType::Runtime;
        return true;
    }
    if (InType.Equals(TEXT("Editor"), ESearchCase::IgnoreCase))
    {
        OutType = EPluginModuleType::Editor;
        return true;
    }
    if (InType.Equals(TEXT("Developer"), ESearchCase::IgnoreCase))
    {
        OutType = EPluginModuleType::Developer;
        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Create Plugin
////////////////////////////////////////////////////////////////////////////////////////////////////

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

            FPluginArchitectDescriptor Descriptor;
            Descriptor.FriendlyName = PluginName;
            Descriptor.Version = TEXT("1.0");
            Descriptor.FileVersion = 3;

            // Default Runtime module
            Descriptor.AddModule(
                PluginName + TEXT("Runtime"),
                EPluginModuleType::Runtime);

            FString Error;
            const FString PluginsDir = FPaths::ProjectPluginsDir();

            if (!FPluginGenerator::GeneratePlugin(Descriptor, PluginsDir, Error))
            {
                PA_LOG(Error, TEXT("Plugin generation failed: %s"), *Error);
            }
            else
            {
                PA_LOG(Log, TEXT("Plugin '%s' created successfully"), *PluginName);
            }
        }));

////////////////////////////////////////////////////////////////////////////////////////////////////
// Add Module
////////////////////////////////////////////////////////////////////////////////////////////////////

static FAutoConsoleCommand CreateModuleCommand(
    TEXT("PluginArchitect.CreateModule"),
    TEXT("Adds a module to an existing plugin. Usage: PluginArchitect.CreateModule <PluginName> <ModuleName> [Runtime|Editor|Developer]"),
    FConsoleCommandWithArgsDelegate::CreateStatic(
        [](const TArray<FString>& Args)
        {
            if (Args.Num() < 2)
            {
                PA_LOG(Error, TEXT("Usage: PluginArchitect.CreateModule <PluginName> <ModuleName> [Type]"));
                return;
            }

            const FString& PluginName = Args[0];
            const FString& ModuleName = Args[1];
            const FString TypeString = Args.Num() >= 3 ? Args[2] : TEXT("Runtime");

            EPluginModuleType ModuleType;
            if (!StringToModuleType(TypeString, ModuleType))
            {
                PA_LOG(Error, TEXT("Invalid module type '%s'. Use Runtime, Editor or Developer"), *TypeString);
                return;
            }

            PA_LOG(Log, TEXT("CreateModule requested"));
            PA_LOG(Log, TEXT("Plugin : %s"), *PluginName);
            PA_LOG(Log, TEXT("Module : %s"), *ModuleName);
            PA_LOG(Log, TEXT("Type   : %s"), *TypeString);

            FString Error;

            if (!FPluginModifier::AddModule(PluginName, ModuleName, ModuleType, Error))
            {
                PA_LOG(Error, TEXT("%s"), *Error);
            }
            else
            {
                PA_LOG(Log, TEXT("Module '%s' added to plugin '%s'"), *ModuleName, *PluginName);
            }
        }));

////////////////////////////////////////////////////////////////////////////////////////////////////

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
