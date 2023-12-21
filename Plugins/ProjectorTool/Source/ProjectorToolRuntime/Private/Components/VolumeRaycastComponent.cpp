// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/VolumeRaycastComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DecoratorVolume.h"

// Sets default values for this component's properties
UVolumeRaycastComponent::UVolumeRaycastComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UVolumeRaycastComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UVolumeRaycastComponent::GenerateNewPoints()
{
	GeneratedPoints.Empty(); // Clear array first before generating new points

	/*while (GeneratedPoints.Num() < Count)
	{
		const float RandFloat = RandStream.FRand();
		float x = 0;
		float y = 0;

		switch (Shape)
		{
		case EProjectionShape::Cylinder :
			{
				const float Radius = Size2D.X / 2;
				const float R = Radius * FMath::Sqrt(RandFloat) ;
				const float Theta = 200 * PI * RandFloat;
				x = R * FMath::Cos(Theta);
				y = R * FMath::Sin(Theta);
				break;
			}

		case EProjectionShape::Cube :
			{
				x = RandStream.FRandRange(-50, 50) * (Size2D.X / 100);
				y = RandStream.FRandRange(-50, 50) * (Size2D.X / 100);
				break;
			}

		case EProjectionShape::Cuboid :
			{
				x = RandStream.FRandRange(-50, 50) * (Size3D.X / 100);
				y = RandStream.FRandRange(-50, 50) * (Size3D.Y / 100);
				break;
			}
		}

		// When hollow is enabled and we do not want the generated points to lie within the zone
		if (Hollow)
		{
			float xThreshold = 0, yThreshold = 0;
			bool bPointIsInCircle = false;

			switch (Shape)
			{
			case EProjectionShape::Cylinder :
				{
					float DistFromCenter = FMath::Sqrt(x*x + y*y); 
					bPointIsInCircle = (DistFromCenter <= HollowSizeF/2);
					break;
				}

			case EProjectionShape::Cube :
				{
					xThreshold = HollowSizeF / 2;
					yThreshold = HollowSizeF / 2;
					break;
				}

			case EProjectionShape::Cuboid :
				{
					xThreshold = HollowSize2D.X / 2;
					yThreshold = HollowSize2D.Y / 2;
					break;
				}
			}
			
			if ((-xThreshold <= x && x <= xThreshold  && -yThreshold  <= y && y <= yThreshold) || bPointIsInCircle)
			{
				continue;
			}
		}
		
		//FVector NewPoint = FVector(x, y, 0);
		GeneratedPoints.Add(NewPoint);
	}*/
}

void UVolumeRaycastComponent::RunLineTrace()
{
	TracedLocations.Empty();
	TracedRotations.Empty();
	
	const FVector ActorLocation = GetOwner()->GetActorLocation();
	const float Z = Shape == EProjectionShape::Cuboid ? Size3D.Z : Size2D.Y; // Uses Vector2D Y value if shape is Cylinder or Cube, else it is using the Vector Z value for Cuboid
	const FVector HalfZOffset = FVector(0, 0, Z / 2);
	FHitResult HitResult;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(GetOwner());
	
	for (FVector CurrPoint : GeneratedPoints)
	{
		// Local Position to World Position == CurrPoint + ActorLocation
		FVector Start =  GetOwner()->GetActorRotation().RotateVector(CurrPoint + HalfZOffset) + ActorLocation;
		FVector End = Start + GetOwner()->GetActorUpVector() * Z * -1;
		GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_WorldStatic, TraceParams);
		
		if (HitResult.bBlockingHit)
		{
			TracedLocations.Add(HitResult.ImpactPoint);
			TracedRotations.Add(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal)); // Creating rotation from surface normal
		}
	}
}

ADecoratorVolume* UVolumeRaycastComponent::GetVolumeOwner()
{
	if (GetOwner()->GetClass()->IsChildOf(ADecoratorVolume::StaticClass()))
	{
		return Cast<ADecoratorVolume>(GetOwner());
	}
	else
	{
		return nullptr;
	}
}

