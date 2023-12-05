// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DecoratorPalette.generated.h"

USTRUCT(BlueprintType)
struct FPaletteStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere)
	UMaterial* Mat;

	UPROPERTY(EditAnywhere)
	FCollisionProfileName CollisionProfile = UCollisionProfile::BlockAll_ProfileName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin = 0))
	int32 Density = 1; // Default density set to 1
	
	UPROPERTY(EditAnywhere)
	bool RandomScale = false;
	
	UPROPERTY(EditAnywhere, meta=(AllowPreserveRatio, EditCondition="RandomScale", EditConditionHides))
	FVector Scale = FVector::One(); // Default scale to (1, 1, 1)

	UPROPERTY(EditAnywhere, meta=(AllowPreserveRatio, EditCondition="RandomScale", EditConditionHides))
	FVector ScaleMin = FVector::One(); // Default to (1, 1, 1)

	UPROPERTY(EditAnywhere, meta=(AllowPreserveRatio, EditCondition="RandomScale", EditConditionHides))
	FVector ScaleMax = FVector::One(); // Default to (1, 1, 1)

	UPROPERTY(EditAnywhere)
	bool RandomRotation = false;
	
	UPROPERTY(EditAnywhere, meta=(AllowPreserveRatio, EditCondition="RandomRotation", EditConditionHides))
	FRotator Rotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, meta=(AllowPreserveRatio, EditCondition="RandomRotation", EditConditionHides))
	FRotator MinRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, meta=(AllowPreserveRatio, EditCondition="RandomRotation", EditConditionHides))
	FRotator MaxRotation = FRotator::ZeroRotator;
};

UCLASS(Abstract, Blueprintable, BlueprintType, CollapseCategories, DefaultToInstanced, EditInlineNew)
class PROJECTORTOOLRUNTIME_API UDecoratorPalette : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere)
	int32 TotalDensity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (TitleProperty="Density: {Density}"))
	TArray<FPaletteStruct> Instances;

public:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& e) override;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetTotalDensity();

	int32 GetNumberOfInstances() const;
	float GetDensityRatioAtIndex(int32 Index);
	FRotator GetRotationAtIndex(int32 Index);
	FVector GetScaleAtIndex(int32 Index);
};
