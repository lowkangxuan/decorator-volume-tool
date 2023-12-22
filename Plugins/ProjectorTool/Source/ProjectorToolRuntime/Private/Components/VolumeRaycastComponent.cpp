// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/VolumeRaycastComponent.h"
#include "Kismet/KismetMathLibrary.h"

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

