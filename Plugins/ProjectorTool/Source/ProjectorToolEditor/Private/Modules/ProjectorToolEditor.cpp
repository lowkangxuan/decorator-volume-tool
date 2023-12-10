// Copyright Epic Games, Inc. All Rights Reserved.

#include "Modules/ProjectorToolEditor.h"

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

	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	EAssetTypeCategories::Type NewCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("Decorator Volume")), FText::FromString("Decorator Volume"));

	TSharedPtr<IAssetTypeActions> PaletteAction = MakeShareable(new FDecoratorPaletteAssetTypeActions(NewCategory));
	TSharedPtr<IAssetTypeActions> VolumeAction = MakeShareable(new FDecoratorVolumeAssetTypeActions(NewCategory));

	AssetTools.RegisterAssetTypeActions(PaletteAction.ToSharedRef());
	AssetTools.RegisterAssetTypeActions(VolumeAction.ToSharedRef());

	CreatedAssetTypeActions.Add(PaletteAction);
	CreatedAssetTypeActions.Add(VolumeAction);
}

void FProjectorToolModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	if (GUnrealEd != nullptr)
	{
		GUnrealEd->UnregisterComponentVisualizer(UDecoratorVolumeVisualizerComponent::StaticClass()->GetFName());
	}

	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();

		for (int32 i = 0; i < CreatedAssetTypeActions.Num(); ++i)
		{
			AssetTools.UnregisterAssetTypeActions(CreatedAssetTypeActions[i].ToSharedRef());
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FProjectorToolModule, ProjectorToolEditor)