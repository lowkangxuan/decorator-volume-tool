// Fill out your copyright notice in the Description page of Project Settings.

#include "..\Public\DecoratorPalette.h"

#include "AssetTypeCategories.h"
#include "HairStrandsInterface.h"
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
	const int32 EditedIndex = e.GetArrayIndex(TEXT("Instances"));
	FPaletteStruct& EditedInstance = Instances[EditedIndex];
	
	if (ChainPropertyName == "ScaleMin")
	{
		EditedInstance.ScaleMin = FMath::Clamp(EditedInstance.ScaleMin, 0.0, EditedInstance.ScaleMax);
	}

	if (ChainPropertyName == "ScaleMax")
	{
		EditedInstance.ScaleMax = FMath::Clamp(EditedInstance.ScaleMax, EditedInstance.ScaleMin, EditedInstance.ScaleMax);
	}

	if (ChainPropertyName == "Mesh")
	{
		EditedInstance.Materials.SetNum(EditedInstance.Mesh->GetStaticMaterials().Num());
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

int32 UDecoratorPalette::GetNumMaterialsAtIndex(int32 Index) const
{
	return Instances[Index].Mesh->GetStaticMaterials().Num();
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


