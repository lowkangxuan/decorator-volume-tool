// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Factories/Palette/DecoratorPaletteAssetTypeActions.h"
#include "Factories/Volume/DecoratorVolumeAssetTypeActions.h"

class FProjectorToolModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	TArray<TSharedPtr<IAssetTypeActions>> CreatedAssetTypeActions;
};
