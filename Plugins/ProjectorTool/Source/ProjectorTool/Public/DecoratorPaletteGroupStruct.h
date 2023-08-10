// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DecoratorPaletteGroupStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTORTOOL_API FDecoratorPaletteGroupStruct
{
	GENERATED_BODY()

public:
	FDecoratorPaletteGroupStruct();
	~FDecoratorPaletteGroupStruct();

	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere)
	UMaterial* Mat;

	UPROPERTY(EditAnywhere)
	FCollisionProfileName CollisionPreset = UCollisionProfile::BlockAll_ProfileName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin = 0))
	int32 Density = 1; // Default density set to 1

	UPROPERTY(EditAnywhere, meta=(AllowPreserveRatio))
	FVector Scale = FVector::One(); // Default scale to (1, 1, 1)
};
