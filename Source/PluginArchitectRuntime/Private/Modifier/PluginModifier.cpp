#include "Modifier/PluginModifier.h"

#include "Serialization/FPluginReader.h"
#include "Serialization/FPluginParser.h"
#include "Generator/PluginGenerator.h"

#include "Misc/Paths.h"
#include "Misc/FileHelper.h"

bool FPluginModifier::AddModule(
    const FString& PluginName,
    const FString& ModuleName,
    EPluginModuleType Type,
    FString& OutError)
{
    // 1) Locate plugin
    const FString PluginDir =
        FPaths::Combine(FPaths::ProjectPluginsDir(), PluginName);

    const FString UPluginPath =
        FPaths::Combine(PluginDir, PluginName + TEXT(".uplugin"));

    // 2) Read json
    TSharedPtr<FJsonObject> Json;
    if (!FPluginReader::Load(UPluginPath, Json, OutError))
        return false;

    // 3) Convert to descriptor
    FPluginArchitectDescriptor Descriptor;
    if (!FPluginParser::ToDescriptor(Json, Descriptor, OutError))
        return false;

    // 4) Add module
    Descriptor.AddModule(ModuleName, Type);

    // 5) Regenerate plugin
    if (!FPluginGenerator::CreateModule(Descriptor, FPaths::ProjectPluginsDir(), OutError))
        return false;

    return true;
}