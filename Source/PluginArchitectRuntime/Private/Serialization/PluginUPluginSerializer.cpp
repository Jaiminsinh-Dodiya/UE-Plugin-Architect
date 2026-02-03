#include "Serialization/PluginUPluginSerializer.h"

#include "Descriptor/PluginArchitectDescriptor.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"

bool FPluginUPluginSerializer::Serialize(
	const FPluginArchitectDescriptor& Descriptor,
	FString& OutJson,
	FString& OutError
)
{
	TSharedRef<FJsonObject> Root = MakeShared<FJsonObject>();

	// ---- Basic plugin fields ----
	Root->SetNumberField(TEXT("FileVersion"), Descriptor.FileVersion);
	Root->SetStringField(TEXT("VersionName"), Descriptor.Version);
	Root->SetStringField(TEXT("FriendlyName"), Descriptor.FriendlyName);
	Root->SetStringField(TEXT("Description"), Descriptor.Description);
	Root->SetStringField(TEXT("Category"), Descriptor.Category);
	Root->SetStringField(TEXT("CreatedBy"), Descriptor.CreatedBy);
	Root->SetStringField(TEXT("CreatedByURL"), Descriptor.CreatedByURL);
	Root->SetStringField(TEXT("DocsURL"), Descriptor.DocsURL);
	Root->SetStringField(TEXT("MarketplaceURL"), Descriptor.MarketplaceURL);
	Root->SetStringField(TEXT("SupportURL"), Descriptor.SupportURL);
	Root->SetBoolField(TEXT("CanContainContent"), Descriptor.bCanContainContent);
	Root->SetBoolField(TEXT("IsBetaVersion"), Descriptor.bIsBetaVersion);
	Root->SetBoolField(TEXT("EnabledByDefault"), Descriptor.bIsEnabledByDefault);
	Root->SetBoolField(TEXT("IsExperimentalVersion"), Descriptor.bIsExperimentalVersion);
	Root->SetBoolField(TEXT("IsHidden"), Descriptor.bIsHidden);

	// ---- Modules ----
	TArray<TSharedPtr<FJsonValue>> ModulesArray;

	for (const FPluginModuleDescriptor& Module : Descriptor.Modules)
	{
		TSharedRef<FJsonObject> ModuleObj = MakeShared<FJsonObject>();
		ModuleObj->SetStringField(TEXT("Name"), Module.Name);
		ModuleObj->SetStringField(TEXT("Type"),
			FPluginModuleDescriptor::ModuleTypeToString(Module.Type));
		ModuleObj->SetStringField(TEXT("LoadingPhase"),
			FPluginModuleDescriptor::ModuleLoadingPhaseToString(Module.LoadingPhase));

		ModulesArray.Add(MakeShared<FJsonValueObject>(ModuleObj));
	}

	Root->SetArrayField(TEXT("Modules"), ModulesArray);

	// ---- Write JSON ----
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutJson);
	if (!FJsonSerializer::Serialize(Root, Writer))
	{
		OutError = TEXT("Failed to serialize .uplugin JSON");
		return false;
	}

	return true;
}
