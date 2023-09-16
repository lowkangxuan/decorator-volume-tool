// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PaletteInstanceStruct.h"
#include "DecoratorPalette.generated.h"

USTRUCT()
struct FMyStruct
{
	GENERATED_BODY()
	
};

UCLASS(Abstract, Blueprintable, BlueprintType, CollapseCategories, DefaultToInstanced, EditInlineNew)
class PROJECTORTOOLRUNTIME_API UDecoratorPalette : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere)
	int32 TotalDensity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (TitleProperty="Density: {Density}"))
	TArray<FPaletteInstanceStruct> Instances;

public:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& e) override;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetTotalDensity();

	int32 GetNumberOfInstances() const;
	float GetDensityRatioAtIndex(int32 Index);
	FRotator GetRotationAtIndex(int32 Index);
	FVector GetScaleAtIndex(int32 Index);
};
