// Fill out your copyright notice in the Description page of Project Settings.


#include "Factories/Volume/ISM/DecoratorVolumeAssetTypeActions.h"
#include "DecoratorVolumeISM.h"

FDecoratorVolumeAssetTypeActions::FDecoratorVolumeAssetTypeActions(EAssetTypeCategories::Type InAssetCategory)
{
	CustomAssetCategory = InAssetCategory;
}

UClass* FDecoratorVolumeAssetTypeActions::GetSupportedClass() const
{
	return ADecoratorVolumeISM::StaticClass();
}

FText FDecoratorVolumeAssetTypeActions::GetName() const
{
	return INVTEXT("Volume ISM");
}

FColor FDecoratorVolumeAssetTypeActions::GetTypeColor() const
{
	return FColor(63, 126, 255);
}

uint32 FDecoratorVolumeAssetTypeActions::GetCategories()
{
	return CustomAssetCategory;
}
