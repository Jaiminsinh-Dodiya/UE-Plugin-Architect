#include "Serialization/UPluginReader.h"

#include "Misc/FileHelper.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"


/**
 * Loads and parses a .uplugin file into a JSON object.
 * @param UPluginPath The file path to the .uplugin file.
 * @param OutDescriptor The output JSON object containing the plugin descriptor data.
 * @param OutError An output string that will contain an error message if loading or parsing fails.
 * @return true if the file was successfully loaded and parsed; false otherwise.
 */
bool FPluginReader::Load
(
    const FString &UPluginPath,
    TSharedPtr<FJsonObject> &OutDescriptor,
    FString &OutError
)
{
    /**
     * Step 1: Load the .uplugin file content into a string.
     * Step 2: Parse the string content as JSON and populate the OutDescriptor.
     * Step 3: Handle any errors that occur during file loading or JSON parsing, populating the OutError string accordingly.
     * Note: This function assumes that the .uplugin file is well-formed and that the JSON structure matches the expected plugin descriptor format. Additional validation may be necessary depending on the use case.
     */
    FString FileContent;
    if (!FFileHelper::LoadFileToString(FileContent, *UPluginPath))
    {
        OutError = FString::Printf(TEXT("Failed to read .uplugin file at path: %s"), *UPluginPath);
        return false;
    }

    // Parse JSON
    TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(FileContent);
    if (!FJsonSerializer::Deserialize(JsonReader, OutDescriptor) || !OutDescriptor.IsValid())
    {
        OutError = FString::Printf(TEXT("Failed to parse JSON from file: %s"), *UPluginPath);
        return false;
    }
    
    return true;
}