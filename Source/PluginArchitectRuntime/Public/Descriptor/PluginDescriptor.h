#pragma once

#include "CoreMinimal.h"
#include "PluginModuleDescriptor.h"

/**
 * Core data structure representing an entire Unreal Engine plugin
 * This is the in-memory representation of a .uplugin file
 * 
 * Design Philosophy:
 * - Pure data container (no file IO, no JSON serialization here)
 * - Self-validating with clear error messages
 * - Serves as the single source of truth for all plugin generation
 * 
 * Future Systems That Will Use This:
 * - UI (for editing plugin properties)
 * - Generator (for creating files)
 * - Validator (for checking correctness)
 * - Serializer (for writing .uplugin JSON)
 */
struct FPluginDescriptor
{
public:
	/** Plugin version (e.g., 1) */
	int32 FileVersion;

	/** Plugin version number (e.g., "1.0") */
	FString Version;

	/** User-friendly plugin name */
	FString FriendlyName;

	/** Plugin description */
	FString Description;

	/** Category for organization */
	FString Category;

	/** Author/creator name */
	FString CreatedBy;

	/** Author URL */
	FString CreatedByURL;

	/** Documentation URL */
	FString DocsURL;

	/** Marketplace URL (if published) */
	FString MarketplaceURL;

	/** Support URL */
	FString SupportURL;

	/** Engine version compatibility (e.g., "5.3.0") */
	FString EngineVersion;

	/** Whether this plugin can contain content */
	bool bCanContainContent;

	/** Whether this is a beta version */
	bool bIsBetaVersion;

	/** Whether this is installed to the engine */
	bool bIsEnabledByDefault;

	/** Whether plugin is experimental */
	bool bIsExperimentalVersion;

	/** Whether this plugin is hidden from the plugin browser */
	bool bIsHidden;

	/** Modules contained in this plugin */
	TArray<FPluginModuleDescriptor> Modules;

	/** Plugins this plugin depends on */
	TArray<FString> Plugins;

	/** Default constructor
	 * Initializes with default values
	 */
	FPluginDescriptor()
		: FileVersion(3)
		, Version(TEXT("1.0"))
		, FriendlyName(TEXT("My Plugin"))
		, Description(TEXT("A description of my plugin"))
		, Category(TEXT("Other"))
		, CreatedBy(TEXT("Author Name"))
		, CreatedByURL(TEXT(""))
		, DocsURL(TEXT(""))
		, MarketplaceURL(TEXT(""))
		, SupportURL(TEXT(""))
		, EngineVersion(TEXT("5.7.2"))
		, bCanContainContent(false)
		, bIsBetaVersion(false)
		, bIsEnabledByDefault(true)
		, bIsExperimentalVersion(false)
		, bIsHidden(false)
	{
	}

	/**
	 * Convenience constructor for quick plugin creation
	 * @param InName - Plugin name (used for FriendlyName)
	 * @param InDescription - Plugin description
	 */
	FPluginDescriptor(const FString& InName, const FString& InDescription)
		: FPluginDescriptor()
	{
		FriendlyName = InName;
		Description = InDescription;
	}

	/**
	 * Validates this plugin descriptor
	 * @param OutErrors - Array to collect all error messages
	 * @return true if valid, false otherwise
	 */
	bool IsValid(TArray<FString>* OutErrors = nullptr) const
	{
		TArray<FString> LocalErrors;

		// Validate friendly name
		if (FriendlyName.IsEmpty())
		{
			LocalErrors.Add(TEXT("Plugin must have a FriendlyName"));
		}

		// Validate version
		if (Version.IsEmpty())
		{
			LocalErrors.Add(TEXT("Plugin must have a Version"));
		}

		// Validate file version
		if (FileVersion <= 0)
		{
			LocalErrors.Add(FString::Printf(TEXT("Invalid FileVersion: %d"), FileVersion));
		}

		// Validate modules
		if (Modules.Num() == 0)
		{
			LocalErrors.Add(TEXT("Plugin must contain at least one module"));
		}
		else
		{
			// Validate each module
			TSet<FString> ModuleNames;
			for (int32 i = 0; i < Modules.Num(); ++i)
			{
				const FPluginModuleDescriptor& Module = Modules[i];
				
				FString ModuleError;
				if (!Module.IsValid(&ModuleError))
				{
					LocalErrors.Add(FString::Printf(TEXT("Module[%d] invalid: %s"), i, *ModuleError));
				}

				// Check for duplicate module names
				if (ModuleNames.Contains(Module.Name))
				{
					LocalErrors.Add(FString::Printf(TEXT("Duplicate module name: %s"), *Module.Name));
				}
				ModuleNames.Add(Module.Name);
			}
		}

		// Collect errors if requested
		if (OutErrors)
		{
			OutErrors->Append(LocalErrors);
		}

		return LocalErrors.Num() == 0;
	}

	/**
	 * Adds a module to this plugin
	 * @param ModuleName - Name of the module
	 * @param ModuleType - Type of the module
	 * @return Reference to the newly added module
	 */
	FPluginModuleDescriptor& AddModule(const FString& ModuleName, EPluginModuleType ModuleType)
	{
		FPluginModuleDescriptor NewModule(ModuleName, ModuleType);
		return Modules.Add_GetRef(NewModule);
	}

	/**
	 * Finds a module by name
	 * @param ModuleName - Name to search for
	 * @return Pointer to module if found, nullptr otherwise
	 */
	FPluginModuleDescriptor* FindModule(const FString& ModuleName)
	{
		for (FPluginModuleDescriptor& Module : Modules)
		{
			if (Module.Name == ModuleName)
			{
				return &Module;
			}
		}
		return nullptr;
	}

	/**
	 * Finds a module by name (const version)
	 */
	const FPluginModuleDescriptor* FindModule(const FString& ModuleName) const
	{
		for (const FPluginModuleDescriptor& Module : Modules)
		{
			if (Module.Name == ModuleName)
			{
				return &Module;
			}
		}
		return nullptr;
	}

	/**
	 * Removes a module by name
	 * @return true if module was found and removed
	 */
	bool RemoveModule(const FString& ModuleName)
	{
		for (int32 i = 0; i < Modules.Num(); ++i)
		{
			if (Modules[i].Name == ModuleName)
			{
				Modules.RemoveAt(i);
				return true;
			}
		}
		return false;
	}

	/**
	 * Gets a summary string for logging
	 */
	FString GetSummary() const
	{
		return FString::Printf(
			TEXT("Plugin '%s' (v%s) with %d module(s)"),
			*FriendlyName,
			*Version,
			Modules.Num()
		);
	}
};
