#include "PluginArchitect.h"
#include "PluginArchitectMacros.h"
#include "HAL/IConsoleManager.h"
#include "Descriptor/PluginArchitectDescriptor.h"
#include "Generator/PluginGenerator.h"
#include "Modifier/PluginModifier.h"
#include "Misc/Paths.h"

#define LOCTEXT_NAMESPACE "FPluginArchitectModule"

/**
 * PluginArchitect module implementation.
 * This module registers console commands for creating plugins and adding modules, and handles plugin generation logic.
 * The actual generation and modification logic is delegated to FPluginGenerator and FPluginModifier classes, which operate on FPluginArchitectDescriptor instances.
 * The console commands provide a simple interface for users to create and modify plugins directly from the Unreal Editor without needing to manually edit JSON files or write code.
 * The module also includes basic logging for startup and shutdown, as well as for command execution to assist with debugging and user feedback.
 */

 /**
  * Helper function to convert a string to EPluginModuleType. Returns true if conversion is successful, false otherwise.
  * This is used to parse the module type from console command arguments when adding a new module to an existing plugin.
  * Supported types are "Runtime", "Editor", and "Developer". The comparison is case-insensitive. If the input string does not match any valid type, the function returns false and OutType is left unchanged.
  * Example usage:
  * EPluginModuleType ModuleType;
  * if (StringToModuleType("Editor", ModuleType))
  * {
  *     // ModuleType is now EPluginModuleType::Editor
  * }
  * else
  * {
  *    // Handle invalid type string
  * }
  */
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

/**
 * Console command to create a new plugin. Usage: PluginArchitect.CreatePlugin <PluginName>
 * This command will create a new plugin with the specified name, generate a default .uplugin
 * file, and create a default Runtime module. The plugin will be created in the project's Plugins directory.
 * Error handling is included to provide feedback if the plugin cannot be created or if generation fails.
 * Note: The actual implementation of plugin generation is handled in the FPluginGenerator::GeneratePlugin function, which is called by this console command.
 */

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


        /**
         * Console command to add a module to an existing plugin. Usage: PluginArchitect.CreateModule <PluginName> <ModuleName> [Runtime|Editor|Developer]
         * Example: PluginArchitect.CreateModule MyPlugin MyPluginEditor Editor
         * This will add a new Editor module named "MyPluginEditor" to the existing "MyPlugin".
         * The module type is optional and defaults to Runtime if not specified.
         * The command will locate the plugin's .uplugin file, parse it, add the new module to the descriptor, and then regenerate the plugin files.
         * Error handling is included to provide feedback if the plugin cannot be found, if the .uplugin file is invalid, or if generation fails.
         * This command allows for dynamic modification of plugins without needing to manually edit JSON files or regenerate the entire plugin from scratch.
         * Note: The actual implementation of adding the module and regenerating the plugin is handled in the FPluginModifier::AddModule function, which is called by this console command.
         */

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

            if (!FPluginModifier::GenerateModule(PluginName, ModuleName, ModuleType, Error))
            {
                PA_LOG(Error, TEXT("%s"), *Error);
            }
            else
            {
                PA_LOG(Log, TEXT("Module '%s' added to plugin '%s'"), *ModuleName, *PluginName);
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
