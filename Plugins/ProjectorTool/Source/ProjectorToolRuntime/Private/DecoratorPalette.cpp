// Fill out your copyright notice in the Description page of Project Settings.

#include "..\Public\DecoratorPalette.h"
#include "GenericPlatform/GenericPlatformMath.h"

#if WITH_EDITOR
	void UDecoratorPalette::PostEditChangeProperty(FPropertyChangedEvent& e)
	{
		Super::PostEditChangeProperty(e);
		const FName PropertyName = (e.Property != NULL ? e.MemberProperty->GetFName() : NAME_None);

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

void UDecoratorPalette::PostEditChangeChainProperty(FPropertyChangedChainEvent& e)
{
		Super::PostEditChangeChainProperty(e);
		const FName ChainPropertyName = (e.Property != NULL ? e.MemberProperty->GetFName() : NAME_None);
		const int32 InstanceIndex = e.GetArrayIndex(TEXT("Instances"));
		
		if (ChainPropertyName == "ScaleMin")
		{
			Instances[InstanceIndex].ScaleMin = FMath::Clamp(Instances[InstanceIndex].ScaleMin, 0.0, Instances[InstanceIndex].ScaleMax);
		}

		if (ChainPropertyName == "ScaleMax")
		{
			Instances[InstanceIndex].ScaleMax = FMath::Clamp(Instances[InstanceIndex].ScaleMax, Instances[InstanceIndex].ScaleMin, Instances[InstanceIndex].ScaleMax);
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

// Returns the scale of the specified instance index
float UDecoratorPalette::GetScaleAtIndex(int32 Index)
{
	return 1;
}

FRotator UDecoratorPalette::GetRotationAtIndex(int32 Index)
{
	return FRotator::ZeroRotator;
}


