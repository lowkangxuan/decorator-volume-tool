#pragma once

class FProjectorToolCommands : public TCommands<FProjectorToolCommands>
{
public:

	FProjectorToolCommands()
		: TCommands<FProjectorToolCommands>(
		TEXT("ProjectorToolMenu"), // Context name for fast lookup
		FText::FromString("Example Menu tool"), // Context name for displaying
		NAME_None,   // No parent context
		FAppStyle::GetAppStyleSetName() // Icon Style Set
		)
	{
	}

	virtual void RegisterCommands() override;

	TSharedPtr<FUICommandInfo> RegenerateAll;
	TSharedPtr<FUICommandInfo> RegenerateAllNewSeed;

	TSharedPtr<FUICommandInfo> BakeAll;
	TSharedPtr<FUICommandInfo> UnbakeAll;
};