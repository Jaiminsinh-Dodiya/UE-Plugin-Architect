#include "Modifier/PluginModifier.h"

#include "Serialization/FPluginReader.h"
#include "Serialization/FPluginParser.h"
#include "Serialization/PluginUPluginSerializer.h"
#include "Generator/PluginModuleGenerator.h"

#include "Misc/Paths.h"
#include "Misc/FileHelper.h"

bool FPluginModifier::GenerateModule(
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

    // Check plugin exists
    if (!FPaths::FileExists(UPluginPath))
    {
        OutError = TEXT("Plugin not found: ") + UPluginPath;
        return false;
    }

    // 2) Read json
    TSharedPtr<FJsonObject> Json;
    if (!FPluginReader::Load(UPluginPath, Json, OutError))
        return false;

    // 3) Convert to descriptor
    FPluginArchitectDescriptor Descriptor;
    if (!FPluginParser::ToDescriptor(Json, Descriptor, OutError))
        return false;

    // 4) Add module to descriptor
    Descriptor.AddModule(ModuleName, Type);

    // 5) Regenerate ONLY module files
    if (!FPluginModuleGenerator::GenerateModule(Descriptor, PluginDir, OutError))
        return false;

    // 6) Rewrite .uplugin
    FString NewJson;
    if (!FPluginUPluginSerializer::Serialize(Descriptor, NewJson, OutError))
        return false;

    if (!FFileHelper::SaveStringToFile(NewJson, *UPluginPath))
    {
        OutError = TEXT("Failed to update .uplugin");
        return false;
    }

    return true;
}
