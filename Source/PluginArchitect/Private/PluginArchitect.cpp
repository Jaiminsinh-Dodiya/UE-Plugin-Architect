// Copyright Epic Games, Inc. All Rights Reserved.

#include "PluginArchitect.h"
#include "PluginArchitectMacros.h"

#define LOCTEXT_NAMESPACE "FPluginArchitectModule"

void FPluginArchitectModule::StartupModule()
{
	PA_LOG(Log, TEXT("PluginArchitect module has started."));
	PA_LOG(Warning, TEXT("This is a warning test"));
	PA_LOG(Error, TEXT("This is an error test (not a real error!)"));
	PA_LOG(Display, TEXT("This is a display message"));
	PA_LOG(Verbose, TEXT("This is verbose - you might not see this"));
}

void FPluginArchitectModule::ShutdownModule()
{
	PA_LOG(Log, TEXT("PluginArchitect module is shutting down."));
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FPluginArchitectModule, PluginArchitect)