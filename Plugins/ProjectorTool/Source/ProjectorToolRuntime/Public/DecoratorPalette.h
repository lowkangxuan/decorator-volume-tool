// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DecoratorPalette.generated.h"

USTRUCT(BlueprintType)
struct FPaletteStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Palette Settings")
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, Category="Palette Settings")
	UMaterialInstance* Mat;

	UPROPERTY(EditAnywhere, Category="Palette Settings")
	FCollisionProfileName CollisionProfile = UCollisionProfile::BlockAll_ProfileName;
	
	UPROPERTY(EditAnywhere, Category="Palette Settings", meta=(ClampMin = 0))
	int32 Density = 1; // Default density set to 1
	
	UPROPERTY(EditAnywhere, Category="Palette Settings")
	bool RandomScale = false;

	UPROPERTY(EditAnywhere, Category="Palette Settings", meta=(EditCondition="RandomScale", EditConditionHides))
	float ScaleMin = 1.0f;

	UPROPERTY(EditAnywhere, Category="Palette Settings", meta=(EditCondition="RandomScale", EditConditionHides))
	float ScaleMax = 1.0f;

	UPROPERTY(EditAnywhere, Category="Palette Settings")
	bool RandomRotation = false;

	UPROPERTY(EditAnywhere, Category="Palette Settings", meta=(AllowPreserveRatio, EditCondition="RandomRotation", EditConditionHides))
	FRotator RotationMin = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, Category="Palette Settings", meta=(AllowPreserveRatio, EditCondition="RandomRotation", EditConditionHides))
	FRotator RotationMax = FRotator::ZeroRotator;
};

UCLASS(Blueprintable, BlueprintType, CollapseCategories)
class PROJECTORTOOLRUNTIME_API UDecoratorPalette : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, Category="Palette Settings")
	int32 TotalDensity;

	UPROPERTY(EditAnywhere, Category="Palette Settings", meta = (TitleProperty="Density: {Density}"))
	TArray<FPaletteStruct> Instances = TArray<FPaletteStruct>();

public:

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& e) override;
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& e) override;
#endif
	
	int32 GetTotalDensity();
	int32 GetNumberOfInstances() const;
	float GetDensityRatioAtIndex(int32 Index);
	float GetScaleAtIndex(int32 Index);
	FRotator GetRotationAtIndex(int32 Index);
};
