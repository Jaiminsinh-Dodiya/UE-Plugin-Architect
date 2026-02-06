#include "Serialization/UPluginWriter.h"

#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "Misc/FileHelper.h"

/**
 * Saves plugin descriptor data to a .uplugin file.
 * @param UPluginPath The file path where the .uplugin file should be saved.
 * @param Descriptor The JSON object containing the plugin descriptor data to be saved.
 * @param OutError An output string that will contain an error message if saving fails.
 * @return true if the file was successfully saved; false otherwise.
 */
bool UPluginWriter::Save
(
    const FString &UPluginPath,
    const TSharedPtr<FJsonObject> &DescriptorObject,
    FString &OutError
)

{
    /**
     * Step 1: Serialize the DescriptorObject to a JSON string.
     * Step 2: Save the JSON string to the specified UPluginPath.
     * Step 3: Handle any errors that occur during serialization or file saving, populating the OutError string accordingly.
     * Note: This function assumes that the DescriptorObject is well-formed and contains valid plugin descriptor data. Additional validation may be necessary depending on the use case.
     */
    FString OutputString;
    TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&OutputString);
    if (!FJsonSerializer::Serialize(DescriptorObject.ToSharedRef(), JsonWriter))
    {
        OutError = TEXT("Failed to serialize plugin descriptor to JSON.");
        return false;
    }

    if (!FFileHelper::SaveStringToFile(OutputString, *UPluginPath))
    {
        OutError = FString::Printf(TEXT("Failed to write .uplugin file at path: %s"), *UPluginPath);
        return false;
    }

    return true;
}