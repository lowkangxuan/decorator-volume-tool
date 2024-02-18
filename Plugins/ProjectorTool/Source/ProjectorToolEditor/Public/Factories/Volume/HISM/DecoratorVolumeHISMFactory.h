// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "DecoratorVolumeHISMFactory.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTORTOOLEDITOR_API UDecoratorVolumeHISMFactory : public UFactory
{
	GENERATED_BODY()

public:
	UDecoratorVolumeHISMFactory();
	UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
