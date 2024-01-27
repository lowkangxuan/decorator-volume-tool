#include "CustomMenu/ProjectToolCommands.h"

#define LOCTEXT_NAMESPACE "ProjectorToolMenu"

void FProjectorToolCommands::RegisterCommands()
{
	UI_COMMAND(RegenerateAll, "Regenerate All", "Regenerate all volume without new seed", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(RegenerateAllNewSeed, "Regenerate All New Seed", "Regenerate all volume with a new seed", EUserInterfaceActionType::Button, FInputChord());

	UI_COMMAND(BakeAll, "Bake All", "Bake all instance proxies into their volume", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(UnbakeAll, "Unbake All", "Unbake all instances into individual proxies", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
