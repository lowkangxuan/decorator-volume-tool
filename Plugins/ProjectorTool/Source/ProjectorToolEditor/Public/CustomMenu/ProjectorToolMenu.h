#pragma once

#include "Modules/IProjectorToolModuleInterface.h"
#include "Enums/VolumeActionsEnum.h"

class FProjectorToolMenu : public IProjectorToolModuleListenerInterface, public TSharedFromThis<FProjectorToolMenu>
{
public:
	virtual ~FProjectorToolMenu() {}

	virtual void OnStartupModule() override;
	virtual void OnShutdownModule() override;

	static UWorld* GetWorld();

protected:
	TSharedPtr<FUICommandList> CommandList;

	TSharedPtr<FExtensibilityManager> LevelEditorMenuExtensibilityManager;
	TSharedPtr<FExtender> MenuExtender;

	static TSharedRef<FWorkspaceItem> MenuRoot;

	void MakePulldownMenu(FMenuBarBuilder &MenuBuilder);
	void FillPulldownMenuSections(FMenuBuilder &MenuBuilder);
	void FillPulldownMenuEntries();
	
	void MapCommands();

	// UI Command functions
	void VolumeCommands(EVolumeActionsEnum Action);
	void RegenerateAll();
	void RegenerateAllNewSeed();
	void BakeAll();
	void UnbakeAll();
};
