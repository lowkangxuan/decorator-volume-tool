// Fill out your copyright notice in the Description page of Project Settings.

#include "..\Public\NewDecoratorPalette.h"
#include "GenericPlatform/GenericPlatformMath.h"

#if WITH_EDITOR
	void UNewDecoratorPalette::PostEditChangeProperty(FPropertyChangedEvent& e)
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

int32 UNewDecoratorPalette::GetTotalDensity() 
{
	return TotalDensity;
}

int32 UNewDecoratorPalette::GetNumberOfInstances() const
{
	return Instances.Num();
}

float UNewDecoratorPalette::GetInstanceDensity(int32 index)
{
	float CalculatedDensity = float(Instances[index].Density) / float(TotalDensity); // int32 to float to show decimal place
	return CalculatedDensity;
}