// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "DecoratorVolumeFactory.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTORTOOLEDITOR_API UDecoratorVolumeFactory : public UFactory
{
	GENERATED_BODY()

public:
	UDecoratorVolumeFactory();
	UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
