// Fill out your copyright notice in the Description page of Project Settings.


#include "DecoratorPalette.h"

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

int32 UDecoratorPalette::GetInstanceDensity(int32 index)
{
	int32 CalculatedDensity = Instances[index].Density / TotalDensity;
	return CalculatedDensity;
}