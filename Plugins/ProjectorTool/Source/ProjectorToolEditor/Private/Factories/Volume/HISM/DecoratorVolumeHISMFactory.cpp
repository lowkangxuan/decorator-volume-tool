// Fill out your copyright notice in the Description page of Project Settings.


#include "Factories/Volume/HISM/DecoratorVolumeHISMFactory.h"
#include "DecoratorVolumeHISM.h"
#include "Kismet2/KismetEditorUtilities.h"

UDecoratorVolumeHISMFactory::UDecoratorVolumeHISMFactory()
{
	SupportedClass = ADecoratorVolumeHISM::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UDecoratorVolumeHISMFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	//return NewObject<ADecoratorVolume>(InParent, InClass, InName, Flags, Context);
	return FKismetEditorUtilities::CreateBlueprint(InClass, InParent, InName, BPTYPE_Normal, UBlueprint::StaticClass(), UBlueprintGeneratedClass::StaticClass());
}
