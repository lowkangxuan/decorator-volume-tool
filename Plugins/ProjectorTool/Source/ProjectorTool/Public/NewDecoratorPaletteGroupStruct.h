// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NewDecoratorPaletteGroupStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTORTOOL_API FNewDecoratorPaletteGroupStruct
{
	GENERATED_BODY()

public:
	FNewDecoratorPaletteGroupStruct();
	~FNewDecoratorPaletteGroupStruct();

	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin = 0))
	int32 Density = 1; // Default density set to 1

	UPROPERTY(EditAnywhere)
	UMaterial* Mat;
};
