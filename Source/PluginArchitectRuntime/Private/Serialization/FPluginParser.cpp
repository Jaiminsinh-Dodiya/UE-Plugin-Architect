#include "Serialization/FPluginParser.h"
#include "DOM/JsonObject.h"
#include "Serialization/JsonTypes.h"

bool FPluginParser::ToDescriptor(
    const TSharedPtr<FJsonObject>& Json,
    FPluginArchitectDescriptor& OutDescriptor,
    FString& OutError)
{
    if (!Json.IsValid())
    {
        OutError = TEXT("Invalid Json object");
        return false;
    }

    // Basic fields
    Json->TryGetStringField(TEXT("FriendlyName"), OutDescriptor.FriendlyName);
    Json->TryGetStringField(TEXT("VersionName"), OutDescriptor.Version);
    Json->TryGetStringField(TEXT("Description"), OutDescriptor.Description);

    // Modules
    const TArray<TSharedPtr<FJsonValue>>* ModulesArray;

    if (Json->TryGetArrayField(TEXT("Modules"), ModulesArray))
    {
        for (const TSharedPtr<FJsonValue>& Value : *ModulesArray)
        {
            const TSharedPtr<FJsonObject> ModuleObj = Value->AsObject();
            if (!ModuleObj.IsValid())
                continue;

            FString Name;
            FString Type;

            ModuleObj->TryGetStringField(TEXT("Name"), Name);
            ModuleObj->TryGetStringField(TEXT("Type"), Type);

            EPluginModuleType ModuleType = EPluginModuleType::Runtime;

            if (Type == TEXT("Editor"))
                ModuleType = EPluginModuleType::Editor;
            else if (Type == TEXT("Developer"))
                ModuleType = EPluginModuleType::Developer;

            OutDescriptor.AddModule(Name, ModuleType);
        }
    }

    return true;
}
