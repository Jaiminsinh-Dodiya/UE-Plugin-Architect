#pragma once

#include "CoreMinimal.h"
/**
 * Utility class responsible for writing plugin descriptor data to .uplugin files.
 * This class abstracts the file I/O and JSON serialization logic, providing a simple interface for saving plugin descriptor data to .uplugin files.
 */
#include "Serialization/JsonTypes.h"

class UPluginWriter
{
    public:

    /**
     * Saves plugin descriptor data to a .uplugin file.
     * @param UPluginPath The file path where the .uplugin file should be saved.
     * @param Descriptor The JSON object containing the plugin descriptor data to be saved.
     * @param OutError An output string that will contain an error message if saving fails.
     * @return true if the file was successfully saved; false otherwise.
     */
    static bool Save
    (
        const FString &UPluginPath,
        const TSharedPtr<FJsonObject> &DescriptorObject,
        FString &OutError
    );
};