// Fill out your copyright notice in the Description page of Project Settings.


#include "Factories/Volume/DecoratorVolumeAssetTypeActions.h"
#include "DecoratorVolume.h"

FDecoratorVolumeAssetTypeActions::FDecoratorVolumeAssetTypeActions(EAssetTypeCategories::Type InAssetCategory)
{
	CustomAssetCategory = InAssetCategory;
}

UClass* FDecoratorVolumeAssetTypeActions::GetSupportedClass() const
{
	return ADecoratorVolume::StaticClass();
}

FText FDecoratorVolumeAssetTypeActions::GetName() const
{
	return INVTEXT("Volume");
}

FText FDecoratorVolumeAssetTypeActions::GetAssetDescription(const FAssetData& AssetData) const
{
	return INVTEXT("Test");
}

FColor FDecoratorVolumeAssetTypeActions::GetTypeColor() const
{
	return FColor::Cyan;
}

uint32 FDecoratorVolumeAssetTypeActions::GetCategories()
{
	return CustomAssetCategory;
}
