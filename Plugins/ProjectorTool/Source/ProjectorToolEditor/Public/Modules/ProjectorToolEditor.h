// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "IProjectorToolModuleInterface.h"

class IAssetTypeActions;

class FProjectorToolEditor : public IProjectorToolModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	virtual void AddModuleListeners() override;

	static inline FProjectorToolEditor& Get()
	{
		return FModuleManager::LoadModuleChecked<FProjectorToolEditor>("ProjectorToolEditor");
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("ProjectorToolEditor");
	}

protected:
	TSharedPtr<FExtensibilityManager> LevelEditorMenuExtensibilityManager;
private:
	TArray<TSharedPtr<IAssetTypeActions>> CreatedAssetTypeActions;
};
