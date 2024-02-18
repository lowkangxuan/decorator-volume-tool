// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

class PROJECTORTOOLEDITOR_API FDecoratorVolumeHISMAssetTypeActions : public FAssetTypeActions_Base
{
public:
	FDecoratorVolumeHISMAssetTypeActions(EAssetTypeCategories::Type InAssetCategory);
	
	UClass* GetSupportedClass() const override;
	FText GetName() const override;
	FColor GetTypeColor() const override;
	uint32 GetCategories() override;

private:
	EAssetTypeCategories::Type CustomAssetCategory;
};
