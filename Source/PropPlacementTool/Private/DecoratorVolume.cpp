// Fill out your copyright notice in the Description page of Project Settings.

#include "DecoratorVolume.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/BillboardComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Math/UnrealMathUtility.h"

#include "Engine/Selection.h"

// Sets default values
ADecoratorVolume::ADecoratorVolume(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SetCanBeDamaged(false);

#if WITH_EDITOR
	// Broadcast whenever the editor selection changes (viewport
	// or world outliner)
	OnSelectionChangedHandle = USelection::SelectionChangedEvent.AddUObject(this, &ADecoratorVolume::OnSelectionChanged);
#endif

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
	USelection::SelectionChangedEvent.Remove(OnSelectionChangedHandle);
	OnSelectionChangedHandle.Reset();
}

#if WITH_EDITOR
void ADecoratorVolume::PostInitProperties()
{
	Super::PostInitProperties();

	InitNewStreamSeed(); //To initialize stream with current seed value when placed in Level
}

void ADecoratorVolume::PostEditChangeProperty(FPropertyChangedEvent& e)
{
	Super::PostEditChangeProperty(e);

	FName PropertyName = (e.Property != NULL ? e.GetPropertyName() : NAME_None);
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

		UE_LOG(LogTemp, Display, TEXT("Decorator Volume: %s_123123"), *FString(PropertyName.ToString()));
		GenerateNewPoints();
	}

	if (PropertyName == "Count")
	{
		RegenerateNoNewSeed();
	}

	// Call UpdateMeshScale() function when the Size property is edited
	if (PropertyName == "X" || PropertyName == "Y")
	{
		UpdateMeshScale();
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

// Set a random int value to Seed and calling SetNewStreamSeed() function
void ADecoratorVolume::RandomizeSeed()
{
	Seed = FMath::RandRange(MinClamp, MaxClamp);
	InitNewStreamSeed();
}

// Initialize RandStream with Seed
void ADecoratorVolume::InitNewStreamSeed()
{
	RandStream.Initialize(Seed);
}

void ADecoratorVolume::GenerateNewPoints()
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
	Function to be called in editor to regenerate new points
	1. Deletes all Instance Mesh Components
	2. Randomize a new Seed
	3. Generate new points based on new Seed
	4. Add new Instance Mesh Components
	5. Draw Debug Lines for visual purposes
*/
void ADecoratorVolume::Regenerate()
{
	DeleteInstMeshComps();
	RandomizeSeed();
	GenerateNewPoints();
	AddInstMeshComps();

	DrawDebugLines(); //For debugging "line rays" for each point generated
}

// Similar to the Regenerate() function, just without randomizing a new seed
void ADecoratorVolume::RegenerateNoNewSeed()
{
	DeleteInstMeshComps();
	GenerateNewPoints();
	AddInstMeshComps();

	DrawDebugLines(); //For debugging "line rays" for each point generated
}

// Add Instance Mesh Component
void ADecoratorVolume::AddInstMeshComps()
{
	for (int i = 0; i < (Palette->GetNumberOfInstances()); ++i)
	{
		UInstancedStaticMeshComponent* InstMeshComp = NewObject<UInstancedStaticMeshComponent>(this, TEXT(""));

		if (!InstMeshComp) return; // Exit the function if for some reason the component is not created
		
		InstMeshComp->OnComponentCreated();
		InstMeshComp->CreationMethod = EComponentCreationMethod::Instance;
		InstMeshComp->AttachToComponent(this->RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		InstMeshComp->SetRelativeLocation(FVector(0)); // Ensure that the component is at the center of the actor
		InstMeshComp->RegisterComponent();
		InstMeshComp->SetStaticMesh(Palette->Instances[i].Mesh); // Setting mesh to c
		AddInstanceComponent(InstMeshComp);

		float c = FMath::RoundHalfFromZero((Count * (Palette->GetInstanceDensity(i))) + PrevCount);
		c = (c >= GeneratedPoints.Num()) ? GeneratedPoints.Num() - 1 : c; // Ensure that the value falls within the range of the array elements
		for (int j = PrevCount; j < c; ++j)
		{
			if (j >= GeneratedPoints.Num()) break;
			UE_LOG(LogTemp, Display, TEXT("J: %d, Num: %d"), j, GeneratedPoints.Num());
			InstMeshComp->AddInstance(FTransform(GeneratedPoints[j]));
		}
		PrevCount = c;
		UE_LOG(LogTemp, Display, TEXT("C: %f, PrevCount: %d, Instance Density: %f"), c, PrevCount, Palette->GetInstanceDensity(i));
	}
	PrevCount = 0;
}

// Get all existing Instance Mesh Component in the actor and delete thems
void ADecoratorVolume::DeleteInstMeshComps()
{
	TArray<UActorComponent*> InstMeshComps = GetComponentsByClass(UInstancedStaticMeshComponent::StaticClass());

	if (InstMeshComps.Num() == 0) return; // Exit the function if no Instance Mesh Component exist
	for (UActorComponent* Components : InstMeshComps)
	{
		Components->UnregisterComponent();
		Components->DestroyComponent();
	}
}

void ADecoratorVolume::DrawDebugLines()
{
	FlushPersistentDebugLines(GetWorld());

	for (FVector CurrPoint : GeneratedPoints)
	{
		FVector ActorLocation = this->GetActorLocation();
		FVector ZOffset = FVector(0, 0, Size.Y / 2);
		FVector LineStart = (CurrPoint + ActorLocation) + ZOffset;
		FVector LineEnd = (CurrPoint + ActorLocation) - ZOffset;
		DrawDebugLine(GetWorld(), LineStart, LineEnd, FColor::Red, false, 1, 0, 1.5);
	}
}

void ADecoratorVolume::DrawDebugLines(FVector Start, FVector End)
{
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1, 0, 1.5);
}

#if WITH_EDITOR
void ADecoratorVolume::OnSelectionChanged(UObject* NewSelection)
{
	TArray<ADecoratorVolume*> SelectedExampleActors;

	// Get ExampleActors from the selection
	USelection* Selection = Cast<USelection>(NewSelection);
	if (Selection != nullptr)
	{
		Selection->GetSelectedObjects<ADecoratorVolume>(SelectedExampleActors);
	}

	// Search the selection for this actor
	for (ADecoratorVolume* SelectedExampleActor : SelectedExampleActors)
	{
		// If our actor is in the selection and was not previously
		// selected, then this selection change marks the actor
		// being selected
		if (SelectedExampleActor == this)
		{
			// Respond to this actor being selected
			UE_LOG(LogTemp, Display, TEXT("Testing: Selected"));
		}
	}

	//if (NewSelection == this)
	//{
	//	UE_LOG(LogTemp, Display, TEXT("Testing: Selected %s"), *this->GetName());
	//}
}
#endif