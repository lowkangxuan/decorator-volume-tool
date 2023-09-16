// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectorToolEditor.h"

#include "DecoratorVolumeVisualizer.h"
#include "DecoratorVolumeVisualizerComponent.h"
#include "UnrealEdGlobals.h"
#include "Editor/UnrealEdEngine.h"

#define LOCTEXT_NAMESPACE "FProjectorToolModule"

void FProjectorToolModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	if (GUnrealEd != nullptr)
	{
		TSharedPtr<FDecoratorVolumeVisualizer> Visualizer = MakeShareable(new FDecoratorVolumeVisualizer);

		if (Visualizer.IsValid())
		{
			GUnrealEd->RegisterComponentVisualizer(UDecoratorVolumeVisualizerComponent::StaticClass()->GetFName(), Visualizer);
			Visualizer->OnRegister();
		}
	}
}

void FProjectorToolModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	if (GUnrealEd != nullptr)
	{
		GUnrealEd->UnregisterComponentVisualizer(UDecoratorVolumeVisualizerComponent::StaticClass()->GetFName());
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FProjectorToolModule, ProjectorToolEditor)