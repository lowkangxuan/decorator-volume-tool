// Fill out your copyright notice in the Description page of Project Settings.


#include "Factories/Palette/DecoratorPaletteFactory.h"
#include "DecoratorPalette.h"
#include "Kismet2/KismetEditorUtilities.h"

UDecoratorPaletteFactory::UDecoratorPaletteFactory()
{
	SupportedClass = UDecoratorPalette::StaticClass();
	bCreateNew = true;
}

UObject* UDecoratorPaletteFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return FKismetEditorUtilities::CreateBlueprint(InClass, InParent, InName, BPTYPE_Normal, UBlueprint::StaticClass(), UBlueprintGeneratedClass::StaticClass());
}
