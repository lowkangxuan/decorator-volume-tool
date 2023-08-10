// Fill out your copyright notice in the Description page of Project Settings.

#include "..\Public\DecoratorVolume.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/BillboardComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Math/UnrealMathUtility.h"

#include "Engine/Selection.h"
#include "GeometryCollection/GeometryCollectionSimulationTypes.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ADecoratorVolume::ADecoratorVolume(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SetCanBeDamaged(false);

	InitNewStreamSeed(); //To initialize stream with current seed value when placed in Level

#pragma region Setting Debug Meshes
	ConstructorHelpers::FObjectFinder<UStaticMesh> DebugCylinder(TEXT("StaticMesh'/Game/DecoratorVolume/Meshes/SM_DebugCylinderMesh.SM_DebugCylinderMesh'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> DebugCube(TEXT("StaticMesh'/Game/DecoratorVolume/Meshes/SM_DebugCubeMesh.SM_DebugCubeMesh'"));
	if (DebugCylinder.Succeeded())
	{
		DebugCylinderMesh = DebugCylinder.Object;
	}

	if (DebugCube.Succeeded())
	{
		DebugCubeMesh = DebugCube.Object;
	}
#pragma endregion Setting Debug Meshes

#pragma region Setting Debug Mesh Material
	// Setting Debug Mesh Material from existing material in project
	ConstructorHelpers::FObjectFinder<UMaterialInterface> MeshMat(TEXT("MaterialInterface'/Game/DecoratorVolume/Materials/MM_EditorWireframe.MM_EditorWireframe'"));
	if (MeshMat.Succeeded())
	{
		DebugMeshMat = MeshMat.Object;
	}
#pragma endregion Setting Debug Mesh Material 

#pragma region Setting RootComponent
	// Setting up SceneComponent as default RootComponent
	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	SetRootComponent(DefaultRoot);
	RootComponent->SetMobility(EComponentMobility::Static);
#pragma endregion Setting RootComponent

#pragma region Setting up DebugMesh parameters
	// Setting up DebugMesh
	DebugMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DebugMesh"));
	DebugMesh->bAffectDistanceFieldLighting = false;
	DebugMesh->bIsEditorOnly = true;
	DebugMesh->SetMobility(EComponentMobility::Static);
	DebugMesh->SetLightingChannels(false, false, false);
	DebugMesh->SetHiddenInGame(true);
	DebugMesh->SetCastShadow(false);
	DebugMesh->SetComponentTickEnabled(false);
	DebugMesh->SetCollisionProfileName("NoCollision", false);
	DebugMesh->SetStaticMesh(DebugCylinderMesh);
	DebugMesh->SetMaterial(0, DebugMeshMat);
	DebugMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
#pragma endregion Setting up DebugMesh parameters

	// Structure to hold one-time initialization
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

#pragma region Editor Sprite Component Stuff
#if WITH_EDITORONLY_DATA
	SpriteComponent = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("Sprite"));

	if (SpriteComponent)
	{
		UE_LOG(LogTemp, Display, TEXT("Test"));
		SpriteComponent->Sprite = ConstructorStatics.EmptyActorTextureObject.Get();
		SpriteComponent->SpriteInfo.Category = ConstructorStatics.ID_Actor;		// Assign sprite category name
		SpriteComponent->SpriteInfo.DisplayName = ConstructorStatics.NAME_Actor;	// Assign sprite display name
		SpriteComponent->bIsScreenSizeScaled = true;
		SpriteComponent->Mobility = EComponentMobility::Static;
		SpriteComponent->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
		SpriteComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	}
#endif
#pragma endregion Editor Sprite Component Stuff
}

void ADecoratorVolume::OnConstruction(const FTransform& Transform)
{
	DrawDebugLines();
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
	GEngine->AddOnScreenDebugMessage(0, 3, FColor::Green, TEXT("Decorator Volume Destroyed!"));
	//UE_LOG(LogTemp, Display, TEXT("Destroyed"));
}


#if WITH_EDITOR
void ADecoratorVolume::PostInitProperties()
{
	Super::PostInitProperties();
}

void ADecoratorVolume::PostEditChangeProperty(FPropertyChangedEvent& e)
{
	Super::PostEditChangeProperty(e);

	const FName PropertyName = (e.Property != NULL ? e.MemberProperty->GetFName() : NAME_None);
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
						DebugMesh->SetStaticMesh(DebugCylinderMesh);
						break;
					}

				case (Cube):
				{
					DebugMesh->SetStaticMesh(DebugCubeMesh);
					break;
				}
			}
		}

		RegeneratePoints();
		RepaintTransform();
	}

	if (PropertyName == "Count")
	{
		TriggerRegeneration(false);
	}
	
	if (PropertyName == "Size")
	{
		UpdateMeshScale();
		RegeneratePoints();
		RepaintTransform();
	}

	if (PropertyName == "AlignToSurface")
	{
		RunLineTrace();
		RepaintTransform();
	}

	if (PropertyName == "DrawProjectionVisualizer")
	{
		
	}
}
#endif

// Scale DebugMesh based on Size.x and Size.y values
void ADecoratorVolume::UpdateMeshScale()
{
	const FVector2D length = FVector2D(Size.X / 100); //Scale of the mesh's XY
	const float height = Size.Y / 100; //Scale of the mesh's Z

	DebugMesh->SetWorldScale3D(FVector(length, height));
}

// Initialize RandStream with Seed
void ADecoratorVolume::InitNewStreamSeed()
{
	RandStream.Initialize(Seed);
}

// Set a random int value to Seed and calling SetNewStreamSeed() function
void ADecoratorVolume::RandomizeSeed()
{
	Seed = FMath::RandRange(MinClamp, MaxClamp);
	InitNewStreamSeed();
}

void ADecoratorVolume::RepaintTransform()
{
	RunLineTrace();
	UpdateInstanceTransform();
}

void ADecoratorVolume::RepaintMeshMaterial()
{
	UpdateInstanceMeshMaterial();
}

// Function button that can be pressed in the Editor to trigger points regeneration
void ADecoratorVolume::Regenerate()
{
	TriggerRegeneration((true));
}

// Regenerate points using Maths and based on the number of Counts and Shape
void ADecoratorVolume::RegeneratePoints()
{
	GeneratedPoints.Empty(); // Clear array first before generating new points
	
	for (int i = 0; i < Count; i++)
	{
		const float RandFloat = RandStream.FRand();
		float x = 0;
		float y = 0;
		FVector NewPoint;

		switch (Shape)
		{
			case Cylinder:
			{
				const float Radius = Size.X / 2;
				const float R = Radius * FMath::Sqrt(RandFloat);
				const float Theta = 200 * PI * RandFloat;
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

	RandStream.Reset(); // Resets the Stream to get back original RANDOM values
}

void ADecoratorVolume::RunLineTrace()
{
	LineTracedLocations.Empty();
	LineTracedRotations.Empty();
	for (FVector CurrPoint : GeneratedPoints)
	{
		FVector ActorLocation = this->GetActorLocation();
		FVector ZOffset = FVector(0, 0, Size.Y / 2);
		FVector Start = (CurrPoint + ActorLocation) + ZOffset;
		FVector End = (CurrPoint + ActorLocation) - ZOffset;
		FHitResult HitResult;
		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(this);
		GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_WorldStatic, TraceParams);
		
		if (HitResult.bBlockingHit)
		{
			//UE_LOG(LogTemp, Display, TEXT("Impact Point: %s, Impact Normal: %s"), *HitResult.ImpactPoint.ToString(), *HitResult.ImpactNormal.ToString());
			LineTracedLocations.Add(HitResult.ImpactPoint);
			LineTracedRotations.Add(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal)); // Creating rotation from surface normal
		}
	}
}

void ADecoratorVolume::TriggerRegeneration(bool NewSeed = false)
{
	// We do not want to call all the functions below if the parameters are kept default
	if (Count == 0 || GetPalette() == nullptr || GetPalette()->GetNumberOfInstances() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Decorator Volume: Please ensure to populate the Count, Palette, and Palette Instances parameters!"));
		return;
	}
	
	if (NewSeed) RandomizeSeed(); // Randomize a new seed if NewSeed param is true
	RegeneratePoints();
	RunLineTrace();
	DeleteInstMeshComponents();  
	AddInstMeshComponents();
	UpdateInstanceMeshMaterial();
	UpdateInstanceTransform();
	
	DrawDebugLines(); //For debugging "line rays" for each point generated
}

// Add Instance Mesh Component
void ADecoratorVolume::AddInstMeshComponents()
{
	TArray<UInstancedStaticMeshComponent*> TempArray;
	
	for (int i = 0; i < GetPalette()->GetNumberOfInstances(); ++i)
	{
		UInstancedStaticMeshComponent* InstMeshComp = NewObject<UInstancedStaticMeshComponent>(this, UInstancedStaticMeshComponent::StaticClass());
		if (!InstMeshComp) return; // Exit the function if for some reason the component is not created
		UE_LOG(LogTemp, Display, TEXT("%s"), *InstMeshComp->GetName());
		InstMeshComp->OnComponentCreated();
		InstMeshComp->CreationMethod = EComponentCreationMethod::Instance;
		InstMeshComp->AttachToComponent(this->RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		InstMeshComp->SetRelativeLocation(FVector(0)); // Ensure that the component is at the center of the actor
		InstMeshComp->RegisterComponent();
		AddInstanceComponent(InstMeshComp);
		TempArray.Add(InstMeshComp);
	}

	Algo::Reverse(TempArray); // Reverse array as GetComponents gets all components in reverse order
	for (int i = 0; i < TempArray.Num(); ++i)
	{
		TempArray[i]->SetCollisionProfileName(GetPalette()->Instances[i].CollisionPreset.Name);
	}
}

// Get all existing Instanced Mesh Component in the actor and delete thems
void ADecoratorVolume::DeleteInstMeshComponents()
{
	TArray<UInstancedStaticMeshComponent*> InstMeshComponents;
	this->GetComponents<UInstancedStaticMeshComponent>(InstMeshComponents);
	
	if (InstMeshComponents.Num() == 0) return; // Exit the function if no Instance Mesh Component exist
	for (UActorComponent* Components : InstMeshComponents)
	{
		Components->UnregisterComponent();
		Components->DestroyComponent();
	}
}

void ADecoratorVolume::UpdateInstanceMeshMaterial()
{
	TArray<UInstancedStaticMeshComponent*> InstMeshComponents;
	this->GetComponents<UInstancedStaticMeshComponent>(InstMeshComponents);
	
	for (int Index = 0; Index < InstMeshComponents.Num(); ++Index)
	{
		UInstancedStaticMeshComponent* CurrComponent = InstMeshComponents[Index];
		UE_LOG(LogTemp, Display, TEXT("%s"), *CurrComponent->GetName());
		CurrComponent->SetStaticMesh(GetPalette()->Instances[Index].Mesh); // Set instance mesh from the current palette index mesh
		CurrComponent->SetMaterial(0, GetPalette()->Instances[Index].Mat); // Set instance material to the current palette index material
	}
}

void ADecoratorVolume::UpdateInstanceTransform()
{
	TArray<UInstancedStaticMeshComponent*> InstMeshComponents;
	this->GetComponents<UInstancedStaticMeshComponent>(InstMeshComponents);
	
	unsigned int PrevCount = 0; // Sort of a "clamp" for the nested loop
	
	for (int Index = 0; Index < InstMeshComponents.Num(); ++Index)
	{
		UInstancedStaticMeshComponent* CurrComponent = InstMeshComponents[Index];
		
		// Delete all existing instances
		if (CurrComponent->GetInstanceCount() > 0)
		{
			CurrComponent->ClearInstances();
		}
		
		unsigned int k = 0;
		float CurrCount = FMath::RoundHalfFromZero((Count * (GetPalette()->GetDensityRatioAtIndex(Index))) + PrevCount);
		CurrCount = FMath::Clamp(CurrCount, 0, LineTracedLocations.Num()); // CLamp value within the number of array elements
		for (int j = PrevCount; j < CurrCount; ++j)
		{
			FTransform InstanceTransform = FTransform(AlignToSurface ? LineTracedRotations[j] : FRotator::ZeroRotator, LineTracedLocations[j], GetPalette()->GetScaleAtIndex(Index));
			CurrComponent->AddInstance(InstanceTransform, true);
			++k;
		}
		
		PrevCount = CurrCount;
	}
}

void ADecoratorVolume::DrawDebugLines()
{
	FlushPersistentDebugLines(GetWorld());
	for (FVector CurrPoint : GeneratedPoints)
	{
		FVector ActorLocation = this->GetActorLocation();
		FVector ZOffset = FVector(0, 0, Size.Y / 2);
		FVector Start = (CurrPoint + ActorLocation) + ZOffset;
		FVector End = (CurrPoint + ActorLocation) - ZOffset;
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1, 0, 1.5);
	}
}

void ADecoratorVolume::DrawDebugLines(FVector Start, FVector End)
{
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1, 0, 1.5);
}

UDecoratorPalette* ADecoratorVolume::GetPalette() const
{
	return Palette.GetDefaultObject();
}
