// Copyright Epic Games, Inc. All Rights Reserved.

#include "PluginArchitect.h"

#define LOCTEXT_NAMESPACE "FPluginArchitectModule"

void FPluginArchitectModule::StartupModule()
{
	UE::Log(LogTemp, Warning, TEXT("Plugin Architect Startup"));
}

void FPluginArchitectModule::ShutdownModule()
{
	UE::Log(LogTemp, Warning, TEXT("Plugin Architect Shutdown"));
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FPluginArchitectModule, PluginArchitect)