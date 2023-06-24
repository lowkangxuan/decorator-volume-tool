// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DecoratorPaletteGroupStruct.h"
#include "DecoratorPalette.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType, CollapseCategories, DefaultToInstanced, EditInlineNew)
class PROPPLACEMENTTOOL_API UDecoratorPalette : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere)
	int32 TotalDensity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDecoratorPaletteGroupStruct> Instances;

public:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& e) override;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetTotalDensity();

};
