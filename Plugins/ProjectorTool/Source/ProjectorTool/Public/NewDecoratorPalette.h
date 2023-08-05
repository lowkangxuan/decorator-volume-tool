// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NewDecoratorPaletteGroupStruct.h"
#include "NewDecoratorPalette.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType, CollapseCategories, DefaultToInstanced, EditInlineNew)
class PROJECTORTOOL_API UNewDecoratorPalette : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere)
	int32 TotalDensity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (TitleProperty="Density: {Density}"))
	TArray<FNewDecoratorPaletteGroupStruct> Instances;

public:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& e) override;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetTotalDensity();

	int32 GetNumberOfInstances() const;
	float GetInstanceDensity(int32 index);
};
