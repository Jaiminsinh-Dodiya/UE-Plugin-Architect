#pragma once

#include "CoreMinimal.h"

/**
 * Represents the type/loading phase of a module
 * Maps directly to .uplugin "Type" field
 */
enum class EPluginModuleType : uint8
{
	Engine,
	Game,
	Developer,
	Editor,
	Runtime,
	ThirdParty
};

/**
 * Represents the loading phase of a module
 * Maps directly to .uplugin "LoadingPhase" field
 */
enum class EPluginModuleLoadingPhase : uint8
{
	PreDefault,
	Default,
	PostDefault,
	PreLoadingScreen,
	None
};

struct FPluginModuleDescriptor
{
public:
	FString Name;
	EPluginModuleType Type;
	EPluginModuleLoadingPhase LoadingPhase;
	TArray<FString> PlatformAllowList;
	TArray<FString> PlatformDenyList;
	TArray<FString> TargetAllowList;
	TArray<FString> TargetDenyList;
	TArray<FString> AdditionalDependencies;

	/** Default constructor
	 * Initializes with default values
	 * Name as empty string
	 * Type as Runtime
	 * LoadingPhase as Default
	 */
	FPluginModuleDescriptor()
		: Name(TEXT(""))
		, Type(EPluginModuleType::Runtime)
		, LoadingPhase(EPluginModuleLoadingPhase::Default)
	{
	}

	/** Convenience constructor for simple module creation
	 * @param InName The name of the module
	 * @param InType The type of the module
	 * @param InLoadingPhase The loading phase of the module
	 */
	FPluginModuleDescriptor(const FString& InName, EPluginModuleType InType, EPluginModuleLoadingPhase InLoadingPhase = EPluginModuleLoadingPhase::Default)
		: Name(InName)
		, Type(InType)
		, LoadingPhase(InLoadingPhase)
	{
	}

	/**
	 * Validates the module descriptor
	 * @param OutErrorMessage The error message if validation fails
	 * @return true if valid, false otherwise
	 */
	bool IsValid(FString* OutErrorMessage = nullptr) const
	{
		if (Name.IsEmpty())
		{
			if (OutErrorMessage)
			{
				*OutErrorMessage = TEXT("Module name is empty");
			}
			return false;
		}

		if (!IsModuleNameValid(Name))
		{
			if (OutErrorMessage)
			{
				*OutErrorMessage = FString::Printf(TEXT("Module name '%s' is invalid"), *Name);
			}
			return false;
		}

		return true;
	}

	/**
	 * Returns a human readable string representation of the module type
	 * @param InType The module type to convert
	 * @return A string representation of the module type
	 */
	static FString ModuleTypeToString(EPluginModuleType InType)
	{
		switch (InType)
		{
		case EPluginModuleType::Engine:
			return TEXT("Engine");

		case EPluginModuleType::Game:
			return TEXT("Game");

		case EPluginModuleType::Developer:
			return TEXT("Developer");

		case EPluginModuleType::Editor:
			return TEXT("Editor");

		case EPluginModuleType::Runtime:
			return TEXT("Runtime");

		case EPluginModuleType::ThirdParty:
			return TEXT("ThirdParty");

		default:
			return TEXT("Unknown");
		}
	}

	/**
	 * Returns a human readable string representation of the loading phase
	 * @param InLoadingPhase The loading phase to convert
	 * @return A string representation of the loading phase
	 */
	static FString ModuleLoadingPhaseToString(EPluginModuleLoadingPhase InLoadingPhase)
	{
		switch (InLoadingPhase)
		{
		case EPluginModuleLoadingPhase::PreDefault:
			return TEXT("PreDefault");

		case EPluginModuleLoadingPhase::Default:
			return TEXT("Default");

		case EPluginModuleLoadingPhase::PostDefault:
			return TEXT("PostDefault");

		case EPluginModuleLoadingPhase::PreLoadingScreen:
			return TEXT("PreLoadingScreen");

		case EPluginModuleLoadingPhase::None:
			return TEXT("None");

		default:
			return TEXT("Unknown");
		}
	}

private:
	/**
	 * Validates if the module name is valid
	 * @param InName The name of the module
	 * @return true if valid, false otherwise
	 */
	bool IsModuleNameValid(const FString& InName) const
	{
		// Module names must start with a letter and can only contain letters, numbers, and underscores
		if (InName.IsEmpty() || !FChar::IsAlpha(InName[0]))
		{
			return false;
		}

		for (TCHAR Char : InName)
		{
			if (!FChar::IsAlnum(Char) && Char != TEXT('_'))
			{
				return false;
			}
		}

		return true;
	}
};
