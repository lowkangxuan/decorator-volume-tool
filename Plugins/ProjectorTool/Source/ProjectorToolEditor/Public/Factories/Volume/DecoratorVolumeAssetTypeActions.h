// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

class PROJECTORTOOLEDITOR_API FDecoratorVolumeAssetTypeActions : public FAssetTypeActions_Base
{
public:
	FDecoratorVolumeAssetTypeActions(EAssetTypeCategories::Type InAssetCategory);
	
	UClass* GetSupportedClass() const override;
	FText GetName() const override;
	FText GetAssetDescription(const FAssetData& AssetData) const override;
	FColor GetTypeColor() const override;
	uint32 GetCategories() override;

private:
	EAssetTypeCategories::Type CustomAssetCategory;
};
