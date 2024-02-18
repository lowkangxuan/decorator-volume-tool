// Fill out your copyright notice in the Description page of Project Settings.


#include "Factories/Volume/HISM/DecoratorVolumeHISMAssetTypeActions.h"
#include "DecoratorVolumeHISM.h"

FDecoratorVolumeHISMAssetTypeActions::FDecoratorVolumeHISMAssetTypeActions(EAssetTypeCategories::Type InAssetCategory)
{
	CustomAssetCategory = InAssetCategory;
}

UClass* FDecoratorVolumeHISMAssetTypeActions::GetSupportedClass() const
{
	return ADecoratorVolumeHISM::StaticClass();
}

FText FDecoratorVolumeHISMAssetTypeActions::GetName() const
{
	return INVTEXT("Volume HISM");
}

FColor FDecoratorVolumeHISMAssetTypeActions::GetTypeColor() const
{
	return FColor(63, 126, 255);
}

uint32 FDecoratorVolumeHISMAssetTypeActions::GetCategories()
{
	return CustomAssetCategory;
}
