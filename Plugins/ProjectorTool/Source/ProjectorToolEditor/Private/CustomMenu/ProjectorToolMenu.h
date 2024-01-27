#pragma once

#include "Modules/IProjectorToolModuleInterface.h"

class FProjectorToolMenu : public IProjectorToolModuleListenerInterface, public TSharedFromThis<FProjectorToolMenu>
{
public:
	virtual ~FProjectorToolMenu() {}

	virtual void OnStartupModule() override;
	virtual void OnShutdownModule() override;

	static UWorld* GetWorld();

	void MakeMenuEntry(FMenuBuilder &MenuBuilder);

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
	void RegenerateAll();
	void RegenerateAllNewSeed();
	void BakeAll();
	void UnbakeAll();
};
