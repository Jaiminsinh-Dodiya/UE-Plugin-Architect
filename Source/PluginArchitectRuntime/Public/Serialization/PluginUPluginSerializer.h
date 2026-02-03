#pragma once

#include "CoreMinimal.h"

struct FPluginArchitectDescriptor;

/**
 * Serializes FPluginArchitectDescriptor into .uplugin JSON text.
 * Pure serialization: no file IO, no validation side-effects.
 */
class FPluginUPluginSerializer
{
public:
	/**
	 * Converts descriptor to .uplugin JSON string.
	 *
	 * @param Descriptor Plugin descriptor data
	 * @param OutJson    Resulting JSON string
	 * @param OutError   Error message on failure
	 * @return true if serialization succeeded
	 */
	static bool Serialize(
		const FPluginArchitectDescriptor& Descriptor,
		FString& OutJson,
		FString& OutError
	);
};