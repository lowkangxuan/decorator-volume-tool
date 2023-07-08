// Fill out your copyright notice in the Description page of Project Settings.

#include "DecoratorPalette.h"
#include "GenericPlatform/GenericPlatformMath.h"

#if WITH_EDITOR
	void UDecoratorPalette::PostEditChangeProperty(FPropertyChangedEvent& e)
	{
		FName PropertyName = (e.Property != NULL ? e.GetPropertyName() : NAME_None);
		UE_LOG(LogTemp, Display, TEXT("Decorator Palette: %s"), *FString(PropertyName.ToString()));

		if (PropertyName == "Instances" || PropertyName == "Density")
		{
			uint32 CurrTotalDensity = 0;

			// Adding all Instance density for Total Density
			for (auto& Elem : Instances)
			{
				CurrTotalDensity += Elem.Density;
			}

			TotalDensity = CurrTotalDensity;
		}
	}
#endif

int32 UDecoratorPalette::GetTotalDensity() 
{
	return TotalDensity;
}

int32 UDecoratorPalette::GetNumberOfInstances()
{
	return Instances.Num();
}

float UDecoratorPalette::GetInstanceDensity(int32 index)
{
	float CalculatedDensity = float(Instances[index].Density) / float(TotalDensity); // int32 to float to show decimal place
	return CalculatedDensity;
}