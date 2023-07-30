// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LootObject.h"
#include "LootDropStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROPPROJECTORTOOL_API FLootDropStruct
{
public:
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<ULootObject*> LootObjects;

	FLootDropStruct();
	~FLootDropStruct();
};
