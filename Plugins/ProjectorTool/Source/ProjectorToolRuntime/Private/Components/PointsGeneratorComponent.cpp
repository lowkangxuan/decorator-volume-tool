// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PointsGeneratorComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values for this component's properties
UPointsGeneratorComponent::UPointsGeneratorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPointsGeneratorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UPointsGeneratorComponent::PostEditChangeProperty(FPropertyChangedEvent& e)
{
	Super::PostEditChangeProperty(e);
	const FName PropertyName = (e.Property != NULL ? e.MemberProperty->GetFName() : NAME_None);

	if (PropertyName == "Size2D" || PropertyName == "Size3D")
	{
		if (Size2D.X <= HollowSizeF + HollowPadding)
		{
			HollowSizeF = FMath::Clamp(HollowSizeF, 0, Size2D.X - HollowPadding);
		}

		if (Size3D.X <= HollowSize2D.X + HollowPadding || Size3D.Y <= HollowSize2D.Y + HollowPadding)
		{
			HollowSize2D.X = FMath::Clamp(HollowSize2D.X, 0, Size3D.X - HollowPadding);
			HollowSize2D.Y = FMath::Clamp(HollowSize2D.Y, 0, Size3D.Y - HollowPadding);
		}
	}

	if (PropertyName == "HollowSizeF" || PropertyName == "HollowSize2D")
	{
		if (Shape == EProjectionShape::Cuboid)
		{
			HollowSize2D.X = FMath::Clamp(HollowSize2D.X, 0, Size3D.X - HollowPadding);
			HollowSize2D.Y = FMath::Clamp(HollowSize2D.Y, 0, Size3D.Y - HollowPadding);
		}
		else
		{
			HollowSizeF = FMath::Clamp(HollowSizeF, 0, GetGenericSize().X - HollowPadding);
		}
	}
}


// Called every frame
void UPointsGeneratorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPointsGeneratorComponent::GeneratePoints(int32 Count, FRandomStream& Stream)
{
	GeneratedPoints.Empty();
	
	while (GeneratedPoints.Num() < Count)
	{
		const float RandFloat = Stream.FRand();
		FVector2D Point;

		// Generating points based on the shape
		switch (Shape)
		{
			case EProjectionShape::Cylinder :
			{
				const float Radius = GetGenericSize().X / 2;
				const float R = Radius * FMath::Sqrt(RandFloat) ;
				const float Theta = 200 * PI * RandFloat;
				Point = FVector2D(
					R * FMath::Cos(Theta),
					R * FMath::Sin(Theta)
					);
				break;
			}

			case EProjectionShape::Cube :
			{
				Point = FVector2D(
					Stream.FRandRange(-50, 50) * (GetGenericSize().X / 100),
					Stream.FRandRange(-50, 50) * (GetGenericSize().X / 100)
					);
				break;
			}

			case EProjectionShape::Cuboid :
			{
				Point = FVector2D(
				Stream.FRandRange(-50, 50) * (GetGenericSize().X / 100),
				Stream.FRandRange(-50, 50) * (GetGenericSize().Y / 100)
				);
				break;
			}
		}

		// When hollow is enabled and we do not want the generated points to lie within the hollowed zone
		if (Hollow)
		{
			bool bPointInHollowCircle = false;
			FVector2D HollowBoundry;

			switch (Shape)
			{
				case EProjectionShape::Cylinder :
				{
					float DistFromCenter = FMath::Sqrt(Point.X * Point.X + Point.Y * Point.Y); // Pythagorean theorem
					bPointInHollowCircle = (DistFromCenter <= HollowSizeF/2);
					break;
				}

				case EProjectionShape::Cube :
				{
					HollowBoundry = FVector2D(
						(HollowSizeF / 2)
						);
					break;
				}

				case EProjectionShape::Cuboid :
				{
					HollowBoundry = FVector2D(
					(HollowSize2D.X / 2),
					(HollowSize2D.Y / 2)
					);
					break;
				}
			}

			// Checking if the generated point is within the hollow zone
			if (Point.X <= HollowBoundry.X && Point.X >= -HollowBoundry.X && Point.Y <= HollowBoundry.Y && Point.Y >= -HollowBoundry.Y || bPointInHollowCircle)
			{
				continue; //Skip this iteration if the point lies within the hollowed threshold
			}
		}

		GeneratedPoints.Add(FVector(Point, 0));
	}
}

void UPointsGeneratorComponent::DoLineTrace()
{
	// Clear array
	LineTracedTransforms.Empty();
	
	const FVector ActorLocation = GetOwner()->GetActorLocation();
	const float Z = GetGenericSize().Z;
	const FVector HalfZOffset = FVector(0, 0, Z / 2);
	FHitResult HitResult;
	FCollisionQueryParams TraceParams;

	// LineTrace to ignore self and specified array of actors
	TraceParams.AddIgnoredActor(GetOwner()); // Ignore owner actor to prevent overlapping
	TraceParams.AddIgnoredActors(ActorsToIgnore);

	for (FVector CurrPoint : GeneratedPoints)
	{
		// Local Position to World Position == CurrPoint + ActorLocation
		FVector Start =  GetOwner()->GetActorRotation().RotateVector(CurrPoint + HalfZOffset) + ActorLocation;
		FVector End = Start + (GetOwner()->GetActorUpVector() * Z) * -1;
		GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_WorldStatic, TraceParams);
		
		if (HitResult.bBlockingHit)
		{
			LineTracedTransforms.Add( FTracedTransform(
				(HitResult.ImpactPoint),
				(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal))
			));
		}
	}
}

FVector UPointsGeneratorComponent::GetGenericSize() const
{
	if (Shape == EProjectionShape::Cuboid) return Size3D;
	return FVector(Size2D.X, Size2D.X, Size2D.Y);
}

FVector UPointsGeneratorComponent::GetGenericHollowSize() const
{
	if (Shape == EProjectionShape::Cuboid) return FVector(HollowSize2D.X, HollowSize2D.Y, Size3D.Z);
	return FVector(HollowSizeF, HollowSizeF, Size2D.Y);
}

