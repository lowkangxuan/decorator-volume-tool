// Fill out your copyright notice in the Description page of Project Settings.

#include "DecoratorVolume.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/BillboardComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ADecoratorVolume::ADecoratorVolume(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SetCanBeDamaged(false);


#pragma region Setting RootComponent
	// Setting up SceneComponent as default RootComponent
	DefaultRoot = CreateDefaultSubobject<USceneComponent>("DefaultRoot");
	SetRootComponent(DefaultRoot);
	RootComponent->SetMobility(EComponentMobility::Static);
#pragma endregion Setting RootComponent

#pragma region Setting VisualizerComponent
	VisualizerComponent = CreateDefaultSubobject<UDecoratorVolumeVisualizerComponent>("Visualizer");
	VisualizerComponent->UpdateSize2D(Size2D);
	VisualizerComponent->UpdateSize3D(Size3D);
	VisualizerComponent->UpdateShape(Shape);
	
#pragma endregion Setting VisualizerComponent

#pragma region Editor Sprite Component Stuff
#if WITH_EDITORONLY_DATA
	SpriteComponent = CreateEditorOnlyDefaultSubobject<UBillboardComponent>("Sprite");
	
	struct FConstructorStatics
	{
		// A helper class object we use to find target UTexture2D object in resource package
		ConstructorHelpers::FObjectFinderOptional<UTexture2D> EmptyActorTextureObject;
		FName ID_Actor;
		FText NAME_Actor;
		FConstructorStatics()
			// Use helper class object to find the texture
			// "/Engine/EditorResources/S_Note" is resource path
			: EmptyActorTextureObject(TEXT("/Engine/EditorResources/EmptyActor"))
			, ID_Actor(TEXT("Actor"))
			, NAME_Actor(NSLOCTEXT("SpriteCategory", "Actor", "Actor"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;
	
	if (SpriteComponent)
	{
		SpriteComponent->Sprite = ConstructorStatics.EmptyActorTextureObject.Get();
		SpriteComponent->SpriteInfo.Category = ConstructorStatics.ID_Actor;		// Assign sprite category name
		SpriteComponent->SpriteInfo.DisplayName = ConstructorStatics.NAME_Actor;	// Assign sprite display name
		SpriteComponent->bIsScreenSizeScaled = true;
		SpriteComponent->Mobility = EComponentMobility::Static;
		SpriteComponent->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
		SpriteComponent->SetupAttachment(RootComponent);
	}
#endif
#pragma endregion Editor Sprite Component Stuff
}

void ADecoratorVolume::OnConstruction(const FTransform& Transform)
{
	// Trying to initialize stream when constructed in Level
	// Also ensure that we are initializing the proper Seed value instead of a default '0'
	/*if (!bIsStreamInitialized)
	{
		InitNewStreamSeed();
		bIsStreamInitialized = true;
	}*/
}

// Called when the game starts or when spawned
void ADecoratorVolume::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void ADecoratorVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADecoratorVolume::BeginDestroy()
{
	Super::BeginDestroy();
}

#if WITH_EDITOR
void ADecoratorVolume::PostEditChangeProperty(FPropertyChangedEvent& e)
{
	Super::PostEditChangeProperty(e);
	
	const FName PropertyName = (e.Property != NULL ? e.MemberProperty->GetFName() : NAME_None);
	UE_LOG(LogTemp, Display, TEXT("%s"), *PropertyName.ToString());
	
	// Generate new points when these properties are edited
	if (PropertyName == "Seed" || PropertyName == "Shape")
	{
		// Init new seed as well when Seed property is edited
		if (PropertyName == "Seed")
		{
			InitNewStreamSeed();
		}

		// Update Visualizer Shape
		if (PropertyName == "Shape")
		{
			VisualizerComponent->UpdateShape(Shape);
		}

		TriggerGeneration();
	}

	if (PropertyName == "Count")
	{
		TriggerGeneration();
	}

	if (PropertyName == "Palette")
	{
		FlushComponents();
		TriggerGeneration();
	}
	
	//  Update Visualizer Size and regenerate the instanced meshes Location
	if (PropertyName == "Size2D" || PropertyName == "Size3D")
	{
		// If current shape is Cuboid, pass in the Vector CuboidSize into the visualizer
		// else, just pass in the default Vector2D Size
		if (Shape == EProjectionShape::Cuboid)
		{
			VisualizerComponent->UpdateSize3D(Size3D);
		}
		else
		{
			VisualizerComponent->UpdateSize2D(Size2D);
		}

		if (Size2D.X <= HollowSizeF + HollowPadding)
		{
			HollowSizeF = FMath::Clamp(HollowSizeF, 0, Size2D.X - HollowPadding);
			VisualizerComponent->UpdateHollowSizeF(HollowSizeF);
		}

		if (Size3D.X <= HollowSize2D.X + HollowPadding || Size3D.Y <= HollowSize2D.Y + HollowPadding)
		{
			HollowSize2D.X = FMath::Clamp(HollowSize2D.X, 0, Size3D.X - HollowPadding);
			HollowSize2D.Y = FMath::Clamp(HollowSize2D.Y, 0, Size3D.Y - HollowPadding);
			VisualizerComponent->UpdateHollowSize2D(HollowSize2D);
		}
	}

	if (PropertyName == "ScaleFromCenter" || PropertyName == "ScaleType")
	{
		TriggerGeneration();
	}

	if (PropertyName == "Hollow")
	{
		TriggerGeneration();
		VisualizerComponent->DrawHollow(Hollow);
	}
	
	if (PropertyName == "HollowSizeF" || PropertyName == "HollowSize2D")
	{
		if (Shape == EProjectionShape::Cuboid)
		{
			HollowSize2D.X = FMath::Clamp(HollowSize2D.X, 0, Size3D.X - HollowPadding);
			HollowSize2D.Y = FMath::Clamp(HollowSize2D.Y, 0, Size3D.Y - HollowPadding);
			VisualizerComponent->UpdateHollowSize2D(HollowSize2D);
		}
		else
		{
			HollowSizeF = FMath::Clamp(HollowSizeF, 0, GetGenericSize().X - HollowPadding);
			VisualizerComponent->UpdateHollowSizeF(HollowSizeF);
		}

		TriggerGeneration();
	}
	
	if (PropertyName == "Alignment")
	{
		TriggerGeneration();
	}

	if (PropertyName == "DrawRaycastLines")
	{
		VisualizerComponent->DrawRaycastLines(DrawRaycastLines);

		if (DrawRaycastLines)
		{
			VisualizerComponent->UpdateStartPoints(GeneratedPoints);
		}
	}
}
#endif

// Initialize RandStream with Seed
void ADecoratorVolume::InitNewStreamSeed()
{
	RandStream.Initialize(Seed);
}

// Set a random int value to Seed and calling SetNewStreamSeed() function
void ADecoratorVolume::RandomizeSeed()
{
	Seed = FMath::RandRange(MinSeedClamp, MaxSeedClamp);
	InitNewStreamSeed();
}

// Regenerate instances transform, mesh, material, collision, etc
// Function that is and only to be called with the Editor button
void ADecoratorVolume::Regenerate()
{
	TriggerGeneration();
}

// Regenerate instances with new Seed
void ADecoratorVolume::GenerateNewSeed()
{
	TriggerGeneration(true);
}

// Completely remove all instances of each Instanced Static Mesh Component(s) in the actor
void ADecoratorVolume::Clear()
{
	TArray<UInstancedStaticMeshComponent*> InstMeshComponents = GetAllInstMeshComponents();

	for (UInstancedStaticMeshComponent* CurrComponent : InstMeshComponents)
	{
		CurrComponent->ClearInstances();
	}

	Modify(); // Marks the actor as dirty
}

// Main function for the generation
void ADecoratorVolume::TriggerGeneration(bool NewSeed)
{
	// We do not want to call all the functions below if the parameters are kept default
	if (Count == 0 || GetPalette() == nullptr || GetPalette()->GetNumberOfInstances() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Decorator Volume: Please ensure to populate the Count, Palette, and Palette Instances parameters!"));
		return;
	}

	// Prevents editor from crashing due to the exceeding array length
	if (GetAllInstMeshComponents().Num() != GetPalette()->Instances.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Palette Instance length changed!"));
		FlushComponents();
	}
	
	if (NewSeed) RandomizeSeed(); // Randomize a new seed if NewSeed param is true

	RandStream.Reset(); // Resets the Stream to get back original RANDOM values
	PointsGeneration();
	RunLineTrace();
	VisualizerComponent->UpdateStartPoints(GeneratedPoints);

	// Reregister Instanced Static Mesh Components and its individual instance Material and Transform
	UpdateInstanceMeshMaterial();
	UpdateInstanceTransform();
	UpdateInstanceCollisionProfile();

	Modify(); // Marks the actor as dirty and require saving
}

// Regenerate points using Maths and based on the number of Counts and Shape
void ADecoratorVolume::PointsGeneration()
{
	GeneratedPoints.Empty(); // Clear array first before generating new points

	while (GeneratedPoints.Num() < Count)
	{
		const float RandFloat = RandStream.FRand();
		float x = 0;
		float y = 0;

		// Generating points based on the shape
		switch (Shape)
		{
			case EProjectionShape::Cylinder :
				{
					const float Radius = GetGenericSize().X / 2;
					const float R = Radius * FMath::Sqrt(RandFloat) ;
					const float Theta = 200 * PI * RandFloat;
					x = R * FMath::Cos(Theta);
					y = R * FMath::Sin(Theta);
					break;
				}

			case EProjectionShape::Cube :
				{
					x = RandStream.FRandRange(-50, 50) * (GetGenericSize().X / 100);
					y = RandStream.FRandRange(-50, 50) * (GetGenericSize().X / 100);
					break;
				}

			case EProjectionShape::Cuboid :
				{
					x = RandStream.FRandRange(-50, 50) * (GetGenericSize().X / 100);
					y = RandStream.FRandRange(-50, 50) * (GetGenericSize().Y / 100);
					break;
				}
		}

		// When hollow is enabled and we do not want the generated points to lie within the hollowed zone
		if (Hollow)
		{
			float xThreshold = 0, yThreshold = 0;
			bool bPointInHollowCircle = false;

			switch (Shape)
			{
				case EProjectionShape::Cylinder :
					{
						float DistFromCenter = FMath::Sqrt(x*x + y*y); 
						bPointInHollowCircle = (DistFromCenter <= HollowSizeF/2);
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
			
			if ((-xThreshold <= x && x <= xThreshold && -yThreshold <= y && y <= yThreshold) || bPointInHollowCircle)
			{
				continue; //Skip this iteration if the point lies within the hollowed threshold
			}
		}
		
		FVector NewPoint = FVector(x, y, 0);
		GeneratedPoints.Add(NewPoint);
	}
}

// Run a set of line traces to gather location and rotation (from hit normal) using generated points and projector size
void ADecoratorVolume::RunLineTrace()
{
	// Clear array
	LineTracedLocations.Empty();
	LineTracedRotations.Empty();
	
	const FVector ActorLocation = GetActorLocation();
	const float Z = GetGenericSize().Z;
	const FVector HalfZOffset = FVector(0, 0, Z / 2);
	FHitResult HitResult;
	FCollisionQueryParams TraceParams;

	// LineTrace to ignore self and specified array of actors
	TraceParams.AddIgnoredActor(this); // Ignore self to prevent overlapping
	TraceParams.AddIgnoredActors(ActorsToIgnore);
	
	for (FVector CurrPoint : GeneratedPoints)
	{
		// Local Position to World Position == CurrPoint + ActorLocation
		FVector Start =  GetActorRotation().RotateVector(CurrPoint + HalfZOffset) + ActorLocation;
		FVector End = Start + (GetActorUpVector() * Z) * -1;
		GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_WorldStatic, TraceParams);
		
		if (HitResult.bBlockingHit)
		{
			LineTracedLocations.Add(HitResult.ImpactPoint);
			LineTracedRotations.Add(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal)); // Creating rotation from surface normal
		}
	}
}

// Removing and Adding new Instanced Static Mesh Component(s)
void ADecoratorVolume::FlushComponents()
{
	// Remove all Instanced Static Mesh Components from the actor
	for (UActorComponent* Components : GetAllInstMeshComponents())
	{
		Components->UnregisterComponent();
		Components->DestroyComponent();
	}

	// Add Instanced Static Mesh Components based on the number of instances in the assigned palette
	for (int i = 0; i < GetPalette()->GetNumberOfInstances(); ++i)
	{
		UInstancedStaticMeshComponent* InstMeshComp = NewObject<UInstancedStaticMeshComponent>(this, UInstancedStaticMeshComponent::StaticClass());

		InstMeshComp->OnComponentCreated();
		InstMeshComp->CreationMethod = EComponentCreationMethod::Instance;
		InstMeshComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		InstMeshComp->RegisterComponent();
		AddInstanceComponent(InstMeshComp);
	}
}

// Gets all existing Instanced Static Mesh Component in the actor and update its mesh and material based on the corresponding instance index
void ADecoratorVolume::UpdateInstanceMeshMaterial()
{
	TArray<UInstancedStaticMeshComponent*> InstMeshComponents = GetAllInstMeshComponents();
	
	for (int Index = 0; Index < InstMeshComponents.Num(); ++Index)
	{
		UInstancedStaticMeshComponent* CurrComponent = InstMeshComponents[Index];
		CurrComponent->SetStaticMesh(GetPalette()->Instances[Index].Mesh); // Set instance mesh from the current palette index mesh
		CurrComponent->SetMaterial(0, GetPalette()->Instances[Index].Mat); // Set instance material to the current palette index material
	}
}

// Delete and Re-adds all instances in each Instanced Static Mesh Component, "updating" each instance with a new set of Transform
void ADecoratorVolume::UpdateInstanceTransform()
{
	const bool UseSurfaceNormal = (Alignment == EInstanceAlignment::SurfaceNormal);
	unsigned int PrevCount = 0; // Sort of a "clamp" for the nested loop
	
	for (int Index = 0; Index < GetAllInstMeshComponents().Num(); ++Index)
	{
		UInstancedStaticMeshComponent* CurrComponent = GetAllInstMeshComponents()[Index];
		
		// Delete all existing instances
		if (CurrComponent->GetInstanceCount() > 0) { CurrComponent->ClearInstances(); }
		
		float CurrCount = FMath::RoundHalfFromZero((Count * (GetPalette()->GetDensityRatioAtIndex(Index))) + PrevCount);
		CurrCount = FMath::Clamp(CurrCount, 0, LineTracedLocations.Num()); // Clamp value within the number of array elements

		const float ScaleMin = GetPalette()->Instances[Index].ScaleMin;
		const float ScaleMax = GetPalette()->Instances[Index].ScaleMax;
		const FRotator RotationMin = GetPalette()->Instances[Index].RotationMin;
		const FRotator RotationMax = GetPalette()->Instances[Index].RotationMax;

		for (int j = PrevCount; j < CurrCount; ++j)
		{
			FRotator FirstRotation = FRotator(FMath::FRandRange(RotationMin.Pitch, RotationMax.Pitch),
											 FMath::FRandRange(RotationMin.Yaw, RotationMax.Yaw) + 90, /* +90 to make the meshes face the correct world orientation */
											 FMath::FRandRange(RotationMin.Roll, RotationMax.Roll)
											 );
			FRotator Rotation = UKismetMathLibrary::ComposeRotators(FirstRotation, UseSurfaceNormal ? LineTracedRotations[j] : FRotator::ZeroRotator);
			FVector Location = LineTracedLocations[j];
			FVector Scale = RandomInstanceScale(LineTracedLocations[j], ScaleMin, ScaleMax);
			
			FTransform InstanceTransform = FTransform(Rotation, Location, Scale);
			CurrComponent->AddInstance(InstanceTransform, true);
		}
		
		PrevCount = CurrCount;
	}
}

// Set the correct collision profile to each mesh instance based on the palette instance
void ADecoratorVolume::UpdateInstanceCollisionProfile()
{
	TArray<UInstancedStaticMeshComponent*> ComponentsArray = GetAllInstMeshComponents();
	
	for (int i = 0; i < GetAllInstMeshComponents().Num(); ++i)
	{
		// Setting each instance a collision profile from their respective palette instance
		ComponentsArray[i]->SetCollisionProfileName(GetPalette()->Instances[i].CollisionProfile.Name);
	}
}

// Generate random scale based on Min and Max value from the palette
FVector ADecoratorVolume::RandomInstanceScale(FVector PointLocation, float ScaleMin, float ScaleMax) const
{
	FVector Scale = FVector::One();
	
	if (ScaleFromCenter)
	{
		// Lerping the scale based on the distance of the point from (0, 0, 0) divided by the size of the volume
		const float DistFromCenter = FMath::Abs(FVector::Distance(FVector::Zero(), PointLocation - GetActorLocation()));
		const float DesiredMin = (ScaleType == EInstanceScaleType::MinToMax ? ScaleMin : ScaleMax);
		const float DesiredMax = (ScaleType == EInstanceScaleType::MinToMax ? ScaleMax : ScaleMin);

		Scale = FVector(FMath::Lerp(DesiredMin, DesiredMax, DistFromCenter/(GetGenericSize().X/2)));
	}
	else
	{
		Scale = FVector(RandStream.FRandRange(ScaleMin, ScaleMax));
	}
	
	return Scale;
}

UDecoratorPalette* ADecoratorVolume::GetPalette() const
{
	return Palette;
}

// Returns the size in 3D vector based on the current projection shape
FVector ADecoratorVolume::GetGenericSize() const
{
	if (Shape == EProjectionShape::Cuboid)
	{
		return Size3D;
	}
	else
	{
		return FVector(Size2D.X, Size2D.X, Size2D.Y);
	}
}

TArray<UInstancedStaticMeshComponent*> ADecoratorVolume::GetAllInstMeshComponents() const
{
	TArray<UInstancedStaticMeshComponent*> Components;
	GetComponents<UInstancedStaticMeshComponent*>(Components);

	return Components;
}
