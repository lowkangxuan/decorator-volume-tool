// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DecoratorPaletteGroupStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROPPLACEMENTTOOL_API FDecoratorPaletteGroupStruct
{
	GENERATED_BODY()

public:
	FDecoratorPaletteGroupStruct();
	~FDecoratorPaletteGroupStruct();

	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin = 0))
	int32 Density = 1; // Defauly density set to 1

	UPROPERTY(EditAnywhere)
	UMaterial* Mat;
};
