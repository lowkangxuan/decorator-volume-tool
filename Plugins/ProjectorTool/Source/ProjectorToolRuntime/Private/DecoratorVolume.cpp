// Fill out your copyright notice in the Description page of Project Settings.

#include "DecoratorVolume.h"
#include "UObject/ConstructorHelpers.h"
#include "DecoratorPalette.h"
#include "Components/BillboardComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/PointsGeneratorComponent.h"
#include "Components/InstanceBakingComponent.h"
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

PointsGeneratorComponent = CreateDefaultSubobject<UPointsGeneratorComponent>("Points Generator Component");

#if WITH_EDITORONLY_DATA
InstanceBakingComponent = CreateEditorOnlyDefaultSubobject<UInstanceBakingComponent>("Instance Baking Component");
#endif
	
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
	UE_LOG(LogTemp, Log, TEXT("%s"), *PropertyName.ToString());
	// Generate new points when these properties are edited
	if (PropertyName == "Seed" || PropertyName == "Shape")
	{
		// Init new seed as well when Seed property is edited
		if (PropertyName == "Seed")
		{
			InitNewStreamSeed();
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

	if (PropertyName == "ScaleFromCenter" || PropertyName == "ScaleType")
	{
		TriggerGeneration();
	}

	if (PropertyName == "Hollow")
	{
		TriggerGeneration();
	}
	
	if (PropertyName == "Alignment")
	{
		TriggerGeneration();
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
#if WITH_EDITORONLY_DATA	// We do not want to call this function if volume is currently unbaked!!!
	if (!InstanceBakingComponent->bIsBaked) { return; }
#endif
	
	for (UInstancedStaticMeshComponent* CurrComponent : GetAllInstMeshComponents())
	{
		CurrComponent->ClearInstances();
	}

	Modify(); // Marks the actor as dirty
}

#if WITH_EDITOR
void ADecoratorVolume::BakeInstances()
{
	InstanceBakingComponent->Bake();
}

void ADecoratorVolume::UnbakeInstances()
{
	InstanceBakingComponent->Unbake();
}
#endif

// Main function for the generation
void ADecoratorVolume::TriggerGeneration(bool NewSeed)
{
	// We do not want to call all the functions below if the parameters are kept default
	if (Count == 0 || GetPalette() == nullptr || GetPalette()->GetNumberOfInstances() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Decorator Volume: Please ensure to populate the Count, Palette, and Palette Instances parameters!"));
		return;
	}

#if WITH_EDITORONLY_DATA
	// Cancel the whole generation process if the volume is currently unbaked for instance editing
	if (!(InstanceBakingComponent->bIsBaked)) return;
#endif
	
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

	// Reregister Instanced Static Mesh Components and its individual instance Material and Transform
	UpdateInstanceMeshMaterial();
	UpdateInstanceTransform();
	UpdateInstanceCollisionProfile();

	Modify(); // Marks the actor as dirty and require saving
}

// Regenerate points using Maths and based on the number of Counts and Shape
void ADecoratorVolume::PointsGeneration()
{
	PointsGeneratorComponent->GeneratePoints(Count, RandStream, GeneratedPoints);
}

// Run a set of line traces to gather location and rotation (from hit normal) using generated points and projector size
void ADecoratorVolume::RunLineTrace()
{
	// Clear array
	LineTracedTransforms.Empty();
	
	const FVector ActorLocation = GetActorLocation();
	const float Z = PointsGeneratorComponent->GetGenericSize().Z;
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
			LineTracedTransforms.Add( FTracedTransform(
				(HitResult.ImpactPoint),
				(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal))
			));
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

	if (!GetPalette()) return; // Prevents weird editor crashes
	
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
		
		float CurrCount = FMath::RoundFromZero((Count * (GetPalette()->GetDensityRatioAtIndex(Index))) + PrevCount);
		CurrCount = FMath::Clamp(CurrCount, 0, LineTracedTransforms.Num()); // Clamp value within the number of array elements
		
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
			FRotator Rotation = UKismetMathLibrary::ComposeRotators(FirstRotation, UseSurfaceNormal ? LineTracedTransforms[j].Rotation : FRotator::ZeroRotator);
			FVector Location = LineTracedTransforms[j].Location;
			FVector Scale = RandomInstanceScale(LineTracedTransforms[j].Location, ScaleMin, ScaleMax);

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
	/*if (ScaleFromCenter)
	{
		// Lerping the scale based on the distance of the point from (0, 0, 0) divided by the size of the volume
		const float DistFromCenter = FMath::Abs(FVector::Distance(FVector::Zero(), PointLocation - GetActorLocation()));
		const float DesiredMin = (ScaleType == EInstanceScaleType::MinToMax ? ScaleMin : ScaleMax);
		const float DesiredMax = (ScaleType == EInstanceScaleType::MinToMax ? ScaleMax : ScaleMin);

		return FVector(FMath::Lerp(DesiredMin, DesiredMax, DistFromCenter/(PointsGeneratorComponent->GetGenericSize().X/2)));
	}*/

	return FVector(RandStream.FRandRange(ScaleMin, ScaleMax));
}

UDecoratorPalette* ADecoratorVolume::GetPalette() const
{
	return Palette;
}

TArray<UInstancedStaticMeshComponent*> ADecoratorVolume::GetAllInstMeshComponents()
{
	TArray<UInstancedStaticMeshComponent*> Components;
	GetComponents<UInstancedStaticMeshComponent*>(Components);

	return Components;
}
