// Fill out your copyright notice in the Description page of Project Settings.


#include "Factories/Palette/DecoratorPaletteAssetTypeActions.h"
#include "DecoratorPalette.h"

FDecoratorPaletteAssetTypeActions::FDecoratorPaletteAssetTypeActions(EAssetTypeCategories::Type InAssetCategory)
{
	CustomAssetCategory = InAssetCategory;
}

UClass* FDecoratorPaletteAssetTypeActions::GetSupportedClass() const
{
	return UDecoratorPalette::StaticClass();
}

FText FDecoratorPaletteAssetTypeActions::GetName() const
{
	return INVTEXT("Palette");
}

FColor FDecoratorPaletteAssetTypeActions::GetTypeColor() const
{
	return FColor::Cyan;
}

uint32 FDecoratorPaletteAssetTypeActions::GetCategories()
{
	return CustomAssetCategory;
}
