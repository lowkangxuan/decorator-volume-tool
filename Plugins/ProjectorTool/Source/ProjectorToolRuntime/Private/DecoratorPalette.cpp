// Fill out your copyright notice in the Description page of Project Settings.

#include "..\Public\DecoratorPalette.h"
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

// Return the total density of the palette
int32 UDecoratorPalette::GetTotalDensity() 
{
	return TotalDensity;
}

// Return total number of instance in the palette
int32 UDecoratorPalette::GetNumberOfInstances() const
{
	return Instances.Num();
}

// Returns the calculated density ratio of the specified instance index
// Density of the specified instance index divided by total density of the palette
float UDecoratorPalette::GetDensityRatioAtIndex(int32 Index)
{
	const float CalculatedDensity = float(Instances[Index].Density) / float(TotalDensity); // int32 to float to show decimal place
	return CalculatedDensity;
}

FRotator UDecoratorPalette::GetRotationAtIndex(int32 Index)
{
	return Instances[Index].RandomScale ? Instances[Index].Rotation : FRotator::ZeroRotator;
}

// Returns the scale of the specified instance index
FVector UDecoratorPalette::GetScaleAtIndex(int32 Index)
{
	return Instances[Index].RandomScale ? Instances[Index].Scale : FVector::One();
}

