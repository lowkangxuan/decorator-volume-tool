// Copyright Epic Games, Inc. All Rights Reserved.

#include "Modules/ProjectorToolEditor.h"
#include "UnrealEdGlobals.h"
#include "Editor/UnrealEdEngine.h"

#include "BaseDecoratorVolume.h"
#include "DecoratorVolumeISM.h"
#include "DecoratorVolumeVisualizer.h"
#include "Components/PointsGeneratorComponent.h"
#include "CustomMenu/ProjectorToolMenu.h"

#include "DetailCustomization/BaseDecoratorVolumeCustomization.h"
#include "DetailCustomization/DecoratorVolumeCustomization.h"

#include "Factories/Palette/DecoratorPaletteAssetTypeActions.h"
#include "Factories/Volume/ISM/DecoratorVolumeAssetTypeActions.h"
#include "Factories/Volume/HISM/DecoratorVolumeHISMAssetTypeActions.h"

void FProjectorToolEditor::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	if (GUnrealEd != nullptr)
	{
		TSharedPtr<FDecoratorVolumeVisualizer> Visualizer = MakeShareable(new FDecoratorVolumeVisualizer);

		if (Visualizer.IsValid())
		{
			GUnrealEd->RegisterComponentVisualizer(UPointsGeneratorComponent::StaticClass()->GetFName(), Visualizer);
			Visualizer->OnRegister();
		}
	}

#pragma region Factory Stuff
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	EAssetTypeCategories::Type NewCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("Decorator Volume")), FText::FromString("Decorator Volume"));

	const TSharedPtr<IAssetTypeActions> PaletteAction = MakeShareable(new FDecoratorPaletteAssetTypeActions(NewCategory));
	const TSharedPtr<IAssetTypeActions> VolumeISMAction = MakeShareable(new FDecoratorVolumeAssetTypeActions(NewCategory));
	const TSharedPtr<IAssetTypeActions> VolumeHISMAction = MakeShareable(new FDecoratorVolumeHISMAssetTypeActions(NewCategory));

	AssetTools.RegisterAssetTypeActions(PaletteAction.ToSharedRef());
	AssetTools.RegisterAssetTypeActions(VolumeISMAction.ToSharedRef());
	AssetTools.RegisterAssetTypeActions(VolumeHISMAction.ToSharedRef());

	CreatedAssetTypeActions.Add(PaletteAction);
	CreatedAssetTypeActions.Add(VolumeISMAction);
	CreatedAssetTypeActions.Add(VolumeHISMAction);
#pragma endregion

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomClassLayout(
		ADecoratorVolumeISM::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FDecoratorVolumeCustomization::MakeInstance)
	);
	
	PropertyModule.RegisterCustomClassLayout(
		ABaseDecoratorVolume::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FBaseDecoratorVolumeCustomization::MakeInstance)
	);
	
	IProjectorToolModuleInterface::StartupModule();
}

void FProjectorToolEditor::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	if (GUnrealEd != nullptr)
	{
		GUnrealEd->UnregisterComponentVisualizer(UPointsGeneratorComponent::StaticClass()->GetFName());
	}

	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();

		for (int32 i = 0; i < CreatedAssetTypeActions.Num(); ++i)
		{
			AssetTools.UnregisterAssetTypeActions(CreatedAssetTypeActions[i].ToSharedRef());
		}
	}

	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomClassLayout(ABaseDecoratorVolume::StaticClass()->GetFName());
		PropertyModule.UnregisterCustomClassLayout(ADecoratorVolumeISM::StaticClass()->GetFName());
	}
}

void FProjectorToolEditor::AddModuleListeners()
{
	IProjectorToolModuleInterface::AddModuleListeners();
	ModuleListeners.Add(MakeShareable(new FProjectorToolMenu));
}
	
IMPLEMENT_MODULE(FProjectorToolEditor, ProjectorToolEditor)