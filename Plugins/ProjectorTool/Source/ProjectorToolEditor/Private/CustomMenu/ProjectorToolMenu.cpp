#include "CustomMenu/ProjectorToolMenu.h"

#include "DecoratorVolumeISM.h"
#include "EngineUtils.h"
#include "Selection.h"
#include "Modules/ProjectorToolEditor.h"
#include "CustomMenu/ProjectToolCommands.h"
#include "Editor/LevelEditor/Public/LevelEditor.h"

#define LOCTEXT_NAMESPACE "ProjectorToolMenu"

TSharedRef<FWorkspaceItem> FProjectorToolMenu::MenuRoot = FWorkspaceItem::NewGroup(FText::FromString("Menu Root"));

void FProjectorToolMenu::OnStartupModule()
{
	CommandList = MakeShareable(new FUICommandList);
	FProjectorToolCommands::Register();
	MapCommands();
	
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	LevelEditorMenuExtensibilityManager = LevelEditorModule.GetMenuExtensibilityManager();
	MenuExtender = MakeShareable(new FExtender);
	MenuExtender->AddMenuBarExtension("Help", EExtensionHook::Before, CommandList, FMenuBarExtensionDelegate::CreateRaw(this, &FProjectorToolMenu::MakePulldownMenu));
	LevelEditorMenuExtensibilityManager->AddExtender(MenuExtender);
	FillPulldownMenuEntries();
}

void FProjectorToolMenu::OnShutdownModule()
{
	FProjectorToolCommands::Unregister();
}

void FProjectorToolMenu::MakePulldownMenu(FMenuBarBuilder& MenuBuilder)
{
	MenuBuilder.AddPullDownMenu(
		FText::FromString("Projector Tool"),
		FText::FromString("List of tools available"),
		FNewMenuDelegate::CreateRaw(this, &FProjectorToolMenu::FillPulldownMenuSections),
		FName("Projector_Tool")
	);
}

// Filling the menu with individual sections
void FProjectorToolMenu::FillPulldownMenuSections(FMenuBuilder& MenuBuilder)
{
	// just a frame for tools to fill in
	MenuBuilder.BeginSection("Regeneration_Section", FText::FromString("Regeneration"));
	MenuBuilder.EndSection();

	MenuBuilder.BeginSection("Baking_Section", FText::FromString("Baking"));
	MenuBuilder.EndSection();
}

// Creating menu entries and adding them to their section
void FProjectorToolMenu::FillPulldownMenuEntries()
{
	struct Local
	{
		static void FillRegeneration(FMenuBuilder& MenuBuilder)
		{
			MenuBuilder.AddMenuEntry(FProjectorToolCommands::Get().RegenerateAll);
			MenuBuilder.AddMenuEntry(FProjectorToolCommands::Get().RegenerateAllNewSeed);
		}

		static void FillBaking(FMenuBuilder& MenuBuilder)
		{
			MenuBuilder.AddMenuEntry(FProjectorToolCommands::Get().BakeAll);
			MenuBuilder.AddMenuEntry(FProjectorToolCommands::Get().UnbakeAll);
		}
	};

	MenuExtender->AddMenuExtension(
		"Regeneration_Section",
		EExtensionHook::First,
		CommandList,
		FMenuExtensionDelegate::CreateStatic(&Local::FillRegeneration));
	
	MenuExtender->AddMenuExtension(
		"Baking_Section",
		EExtensionHook::First,
		CommandList,
		FMenuExtensionDelegate::CreateStatic(&Local::FillBaking));
}

void FProjectorToolMenu::MapCommands()
{
	const auto& Commands = FProjectorToolCommands::Get();

	CommandList->MapAction(
		Commands.RegenerateAll,
		FExecuteAction::CreateSP(this, &FProjectorToolMenu::RegenerateAll),
		FCanExecuteAction());

	CommandList->MapAction(
		Commands.RegenerateAllNewSeed,
		FExecuteAction::CreateSP(this, &FProjectorToolMenu::RegenerateAllNewSeed),
		FCanExecuteAction());

	CommandList->MapAction(
		Commands.BakeAll,
		FExecuteAction::CreateSP(this, &FProjectorToolMenu::BakeAll),
		FCanExecuteAction());

	CommandList->MapAction(
		Commands.UnbakeAll,
		FExecuteAction::CreateSP(this, &FProjectorToolMenu::UnbakeAll),
		FCanExecuteAction());
}

#pragma region Commands
void FProjectorToolMenu::RegenerateAll()
{
	GEditor->GetSelectedActors()->BeginBatchSelectOperation();
	GEditor->SelectNone(false, true);
	for( ADecoratorVolumeISM* Volume : TActorRange<ADecoratorVolumeISM>(GetWorld()))
	{
		Volume->Regenerate();
	}
	GEditor->GetSelectedActors()->EndBatchSelectOperation();
}

void FProjectorToolMenu::RegenerateAllNewSeed()
{
	GEditor->GetSelectedActors()->BeginBatchSelectOperation();
	GEditor->SelectNone(false, true);
	for( ADecoratorVolumeISM* Volume : TActorRange<ADecoratorVolumeISM>(GetWorld()))
	{
		Volume->GenerateNewSeed();
	}
	GEditor->GetSelectedActors()->EndBatchSelectOperation();
}

void FProjectorToolMenu::BakeAll()
{
	GEditor->GetSelectedActors()->BeginBatchSelectOperation();
	GEditor->SelectNone(false, true);
	for( ADecoratorVolumeISM* Volume : TActorRange<ADecoratorVolumeISM>(GetWorld()))
	{
		Volume->BakeInstances();
	}
	GEditor->GetSelectedActors()->EndBatchSelectOperation();
}

void FProjectorToolMenu::UnbakeAll()
{
	GEditor->GetSelectedActors()->BeginBatchSelectOperation();
	GEditor->SelectNone(false, true);
	for( ADecoratorVolumeISM* Volume : TActorRange<ADecoratorVolumeISM>(GetWorld()))
	{
		Volume->UnbakeInstances();
	}
	GEditor->GetSelectedActors()->EndBatchSelectOperation();
}
#pragma endregion

UWorld* FProjectorToolMenu::GetWorld()
{
	return GEditor->GetEditorWorldContext().World();
}

#undef LOCTEXT_NAMESPACE
