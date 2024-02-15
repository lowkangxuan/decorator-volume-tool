// Fill out your copyright notice in the Description page of Project Settings.


#include "Factories/Volume/ISM/DecoratorVolumeFactory.h"
#include "DecoratorVolumeISM.h"
#include "Kismet2/KismetEditorUtilities.h"

UDecoratorVolumeFactory::UDecoratorVolumeFactory()
{
	SupportedClass = ADecoratorVolumeISM::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UDecoratorVolumeFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	//return NewObject<ADecoratorVolume>(InParent, InClass, InName, Flags, Context);
	return FKismetEditorUtilities::CreateBlueprint(InClass, InParent, InName, BPTYPE_Normal, UBlueprint::StaticClass(), UBlueprintGeneratedClass::StaticClass());
}
