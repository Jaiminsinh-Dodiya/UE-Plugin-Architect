#pragma once

#include "CoreMinimal.h"

/**
 * Utility class responsible for loading and parsing .uplugin files.
 * This class abstracts the file I/O and JSON parsing logic, providing a simple interface for retrieving plugin descriptor data from .uplugin files.
 */
#include "Serialization/JsonTypes.h"
class FJsonObject;

/**
 * FUnrealPluginReader is a utility class that provides functionality to load and parse .uplugin files, converting their contents into JSON objects for further processing.
 * This class is designed to be used by the plugin generation system to read existing .uplugin files and extract necessary information for plugin management and generation tasks.
 */
class FPluginReader
{
    public:

    /**
     * Loads and parses a .uplugin file into a JSON object.
     * @param UPluginPath The file path to the .uplugin file.
     * @param OutDescriptor The output JSON object containing the plugin descriptor data.
     * @param OutError An output string that will contain an error message if loading or parsing fails.
     */
    static bool Load
    (
        const FString &UPluginpath,
        TSharedPtr<FJsonObject> &OutDescriptor,
        FString &OutError
    );
};