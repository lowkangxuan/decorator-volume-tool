// Fill out your copyright notice in the Description page of Project Settings.


#include "DecoratorPalette.h"

#if WITH_EDITOR
	void UDecoratorPalette::PostEditChangeProperty(FPropertyChangedEvent& e)
	{
		FName PropertyName = (e.Property != NULL ? e.GetPropertyName() : NAME_None);
		UE_LOG(LogTemp, Display, TEXT("Test Debug: %s"), *FString(PropertyName.ToString()));
		if (PropertyName == "Instances") 
		{
			UE_LOG(LogTemp, Display, TEXT("Hi"));
		}

		if (PropertyName == "Density")
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