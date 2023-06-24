// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LootObject.generated.h"

UENUM(BlueprintType)
enum class ELootType : uint8
{
	Undefined UMETA(DisplayName = "Undefined", Tooltip = "Please define the loot type!"),
	Health UMETA(DisplayName = "Health"),
	Ammo UMETA(DisplayName = "Ammo"),
	Perk UMETA(DisplayName = "Perk"),
	Core UMETA(DisplayName = "Core"),
	Weapon UMETA(DisplayName = "Weapon"),
};

UCLASS(Abstract, DefaultToInstanced, Blueprintable, EditInlineNew)
class PROPPLACEMENTTOOL_API ULootObject : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Loot", meta=(DisplayPriority="0"))
	ELootType LootType = ELootType::Undefined;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Loot", meta = (DisplayPriority = "0"))
	int32 MinDropAmount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Loot", meta = (DisplayPriority = "0"))
	int32 MaxDropAmount = 0;
};
