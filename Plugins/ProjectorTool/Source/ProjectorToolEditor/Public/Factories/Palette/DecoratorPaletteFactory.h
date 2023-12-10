// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "DecoratorPaletteFactory.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTORTOOLEDITOR_API UDecoratorPaletteFactory : public UFactory
{
	GENERATED_BODY()

public:
	UDecoratorPaletteFactory();
	UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
