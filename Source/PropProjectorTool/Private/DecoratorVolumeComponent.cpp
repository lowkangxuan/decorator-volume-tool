// Fill out your copyright notice in the Description page of Project Settings.


#include "DecoratorVolumeComponent.h"

#include "ComponentUtils.h"
#include "Components/InstancedStaticMeshComponent.h"

// Sets default values for this component's properties
UDecoratorVolumeComponent::UDecoratorVolumeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UDecoratorVolumeComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UDecoratorVolumeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

#if WITH_EDITOR
void UDecoratorVolumeComponent::PostInitProperties()
{
	Super::PostInitProperties();

	InitNewStreamSeed(); //To initialize stream with current seed value when placed in Level
}

void UDecoratorVolumeComponent::PostEditChangeProperty(FPropertyChangedEvent& e)
{
	Super::PostEditChangeProperty(e);

	const FName PropertyName = (e.Property != NULL ? e.GetPropertyName() : NAME_None);
	UE_LOG(LogTemp, Display, TEXT("Decorator Volume: %s"), *FString(PropertyName.ToString()));
	
	// Generate new points when these properties are edited
	if (PropertyName == "Seed" || PropertyName == "Shape")
	{
		// Init new seed as well when Seed property is edited
		if (PropertyName == "Seed")
		{
			InitNewStreamSeed();
		}

		// Swap the mesh of DebugMesh when the Shape enum is edited
		if (PropertyName == "Shape")
		{
			switch (Shape)
			{
			case (Cylinder):
				{
					//DebugMesh->SetStaticMesh(DebugCylinderMesh);
					break;
				}

			case (Cube):
				{
					//DebugMesh->SetStaticMesh(DebugCubeMesh);
					break;
				}
			}
		}

		RegeneratePoints();
	}

	if (PropertyName == "Count")
	{
		TriggerRegeneration(false);
	}

	// Call UpdateMeshScale() function when the Size property is edited
	if (PropertyName == "X" || PropertyName == "Y")
	{
		UpdateMeshScale();
		RegeneratePoints();
		UpdateInstMeshComps();
	}
}
#endif

// Scale DebugMesh based on Size.x and Size.y values
void UDecoratorVolumeComponent::UpdateMeshScale()
{
	const FVector2D length = FVector2D(Size.X / 100); //Scale of the mesh's XY
	const float height = Size.Y / 100; //Scale of the mesh's Z

	//DebugMesh->SetWorldScale3D(FVector(length, height));
}

// Set a random int value to Seed and calling SetNewStreamSeed() function
void UDecoratorVolumeComponent::RandomizeSeed()
{
	Seed = FMath::RandRange(MinClamp, MaxClamp);
	InitNewStreamSeed();
}

// Initialize RandStream with Seed
void UDecoratorVolumeComponent::InitNewStreamSeed()
{
	RandStream.Initialize(Seed);
}

// Function button that can be pressed in the Editor to trigger points regeneration
void UDecoratorVolumeComponent::Regenerate()
{
	TriggerRegeneration((true));
}

// Regenerate points using Maths and based on the number of Counts and Shape of the volume
void UDecoratorVolumeComponent::RegeneratePoints()
{
	GeneratedPoints.Empty(); // Clear array first before generating new points
	
	for (int i = 0; i < Count; i++)
	{
		//UE_LOG(LogTemp, Display, TEXT("Count: %d"), i);
		float RandFloat = RandStream.FRand();
		float x;
		float y;
		FVector NewPoint;

		switch (Shape)
		{
			case Cylinder:
			{
				float Radius = Size.X / 2;
				float R = Radius * FMath::Sqrt(RandFloat);
				float Theta = 200 * PI * RandFloat;
				x = R * FMath::Cos(Theta);
				y = R * FMath::Sin(Theta);
				break;
			}

			case Cube:
			{
				x = RandStream.FRandRange(-50, 50) * (Size.X / 100);
				y = RandStream.FRandRange(-50, 50) * (Size.X / 100);
				break;
			}
		}

		NewPoint = FVector(x, y, 0);
		GeneratedPoints.Add(NewPoint);
	}
}

/*
	1. Deletes all Instance Mesh Components
	2. Randomize a new Seed if NewSeed param is True
	3. Generate new points based on new Seed
	4. Add new Instance Mesh Component(s)
	5. Draw Debug Lines for visual purposes
*/
void UDecoratorVolumeComponent::TriggerRegeneration(bool NewSeed = false)
{
	if (NewSeed) RandomizeSeed(); // Randomize a new seed if NewSeed param is true
	RegeneratePoints();
	DeleteInstMeshComps();  
	AddInstMeshComps();     
	UpdateInstMeshComps();
	DrawDebugLines(); //For debugging "line rays" for each point generated
}

// Add Instance Mesh Component
void UDecoratorVolumeComponent::AddInstMeshComps()
{
	for (int i = 0; i < (Palette->GetNumberOfInstances()); ++i)
	{
		UInstancedStaticMeshComponent* InstMeshComp = NewObject<UInstancedStaticMeshComponent>(this, TEXT("Inst Component"));
		if (!InstMeshComp) return; // Exit the function if for some reason the component is not created
		
		InstMeshComp->OnComponentCreated();
		InstMeshComp->CreationMethod = EComponentCreationMethod::Instance;
		InstMeshComp->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
		InstMeshComp->SetRelativeLocation(FVector(0)); // Ensure that the component is at the center of the actor
		InstMeshComp->RegisterComponent();
		GetOwner()->AddInstanceComponent(InstMeshComp);
	}
}

// Get all existing Instance Mesh Component in the actor and delete thems
void UDecoratorVolumeComponent::DeleteInstMeshComps()
{
	TArray<UInstancedStaticMeshComponent*> InstMeshComps;
	GetOwner()->GetComponents<UInstancedStaticMeshComponent>(InstMeshComps);
	
	if (InstMeshComps.Num() == 0) return; // Exit the function if no Instance Mesh Component exist
	for (UActorComponent* Components : InstMeshComps)
	{
		Components->UnregisterComponent();
		Components->DestroyComponent();
	}
}

// Add or Update instances in the InstancedMeshComponent(s)
void UDecoratorVolumeComponent::UpdateInstMeshComps()
{
	TArray<UInstancedStaticMeshComponent*> InstMeshComps;
	GetOwner()->GetComponents<UInstancedStaticMeshComponent>(InstMeshComps);
	unsigned int  i = 0; // Fake "index" for the for-loop below
	unsigned int PrevCount = 0; // Sort of a "range" for the nested loop
	
	/*
	 * Number of elements in InstMeshComps is equivalent to the number of instances in the Palette
	 * InstMeshComps.Num() == Palette->GetNumberOfInstances()
	*/
	for (UInstancedStaticMeshComponent* Component : InstMeshComps)
	{
		Component->SetStaticMesh(Palette->Instances[i].Mesh); // Set instance mesh to the current palette index mesh
		unsigned int k = 0;
		
		float CurrCount = FMath::RoundHalfFromZero((Count * (Palette->GetInstanceDensity(i))) + PrevCount);
		CurrCount = FMath::Clamp(CurrCount, 0, GeneratedPoints.Num()); // Ensure that the value falls within the range of the array elements
		for (int j = PrevCount; j < CurrCount; ++j)
		{
			// If the number of instances is equivalent to the desired count, then update the instance Transform
			// else, add a new instance Transform
			if (Component->GetInstanceCount() == (CurrCount - PrevCount))
			{
				Component->UpdateInstanceTransform(k, FTransform(GeneratedPoints[j]), false, false, true);
			}
			else
			{
				Component->AddInstance(FTransform(GeneratedPoints[j]));
			}

			++k;
			UE_LOG(LogTemp, Display, TEXT("%d"), k);
		}
		UE_LOG(LogTemp, Display, TEXT("CurrCount: %f, PrevCount: %d, Instance Density: %f"), CurrCount, PrevCount, Palette->GetInstanceDensity(i));
		PrevCount = CurrCount;
		
		++i; // Increment "index"
	}
}

void UDecoratorVolumeComponent::RunLineTraces()
{
	LineTracedLocations.Empty();
	for (FVector CurrPoint : GeneratedPoints)
	{
		FVector ActorLocation = GetOwner()->GetActorLocation();
		FVector ZOffset = FVector(0, 0, Size.Y / 2);
		FVector Start = (CurrPoint + ActorLocation) + ZOffset;
		FVector End = (CurrPoint + ActorLocation) - ZOffset;
		FHitResult HitResult;
		FCollisionQueryParams TraceParams;
		GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_WorldStatic, TraceParams);
		
		if (HitResult.bBlockingHit)
		{
			UE_LOG(LogTemp, Display, TEXT("Hit Result: %s"), *HitResult.ImpactPoint.ToString());
			LineTracedLocations.Add(HitResult.ImpactPoint);
		}
	}
}

void UDecoratorVolumeComponent::DrawDebugLines()
{
	FlushPersistentDebugLines(GetWorld());
	for (FVector CurrPoint : GeneratedPoints)
	{
		FVector ActorLocation = GetOwner()->GetActorLocation();
		FVector ZOffset = FVector(0, 0, Size.Y / 2);
		FVector Start = (CurrPoint + ActorLocation) + ZOffset;
		FVector End = (CurrPoint + ActorLocation) - ZOffset;
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1, 0, 1.5);
	}
}

void UDecoratorVolumeComponent::DrawDebugLines(FVector Start, FVector End)
{
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1, 0, 1.5);
}