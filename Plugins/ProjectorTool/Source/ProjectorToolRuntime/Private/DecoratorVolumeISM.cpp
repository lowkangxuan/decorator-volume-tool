// Fill out your copyright notice in the Description page of Project Settings.

#include "DecoratorVolumeISM.h"
#include "UObject/ConstructorHelpers.h"
#include "DecoratorPalette.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/PointsGeneratorComponent.h"
#include "Components/InstanceBakingComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ADecoratorVolumeISM::ADecoratorVolumeISM(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SetCanBeDamaged(false);

#if WITH_EDITORONLY_DATA
InstanceBakingComponent = CreateEditorOnlyDefaultSubobject<UInstanceBakingComponent>("Instance Baking Component");
#endif
}

// Called when the game starts or when spawned
void ADecoratorVolumeISM::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void ADecoratorVolumeISM::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#if WITH_EDITOR
void ADecoratorVolumeISM::PostEditChangeProperty(FPropertyChangedEvent& e)
{
	Super::PostEditChangeProperty(e);
	
	const FName PropertyName = (e.Property != NULL ? e.MemberProperty->GetFName() : NAME_None);

	if (PropertyName == "Palette")
	{
		FlushComponents();
		TriggerGeneration();
	}
}
#endif

// Completely remove all instances of each Instanced Static Mesh Component(s) in the actor
void ADecoratorVolumeISM::Clear()
{
	// We do not want to call this function if volume is currently unbaked!!!
#if WITH_EDITORONLY_DATA
	if (!InstanceBakingComponent->bIsBaked) { return; }
#endif
	
	bIsCleared = true;
	for (UInstancedStaticMeshComponent* CurrComponent : GetAllInstMeshComponents())
	{
		CurrComponent->ClearInstances();
	}

	Modify(); // Marks the actor as dirty
}

#if WITH_EDITOR
void ADecoratorVolumeISM::BakeInstances()
{
	InstanceBakingComponent->Bake();
}

void ADecoratorVolumeISM::UnbakeInstances()
{
	if (!GetPalette() || Count <= 0) return; // Unbaking shouldn't be possible there aren't any palette or 0 count input
	InstanceBakingComponent->Unbake();
}
#endif

// Main function for the generation
void ADecoratorVolumeISM::TriggerGeneration(bool NewSeed)
{
	Super::TriggerGeneration(NewSeed);
	
	// We do not want to call all the functions below if the parameters are kept default
	if (!GetPalette() || GetPalette()->GetNumberOfInstances() == 0) return;

#if WITH_EDITORONLY_DATA
	// Cancel the whole generation process if the volume is currently unbaked for instance editing
	if (!(InstanceBakingComponent->bIsBaked)) return;
#endif

	if (bIsCleared) { bIsCleared = false; }
	
	// Prevents editor from crashing due to the exceeding array length
	if (GetAllInstMeshComponents().Num() != GetPalette()->Instances.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Palette Instance length changed!"));
		FlushComponents();
	}

	// Reregister Instanced Static Mesh Components and its individual instance Material and Transform
	UpdateInstanceMeshMaterial();
	UpdateInstanceTransform();
	UpdateInstanceCollisionProfile();
}

// Removing and Adding new Instanced Static Mesh Component(s)
void ADecoratorVolumeISM::FlushComponents()
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
		UInstancedStaticMeshComponent* InstMeshComp = ConstructISMComponent();

		InstMeshComp->OnComponentCreated();
		InstMeshComp->CreationMethod = EComponentCreationMethod::Instance;
		InstMeshComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		InstMeshComp->RegisterComponent();
		AddInstanceComponent(InstMeshComp);
	}
}

// Gets all existing Instanced Static Mesh Component in the actor and update its mesh and material based on the corresponding instance index
void ADecoratorVolumeISM::UpdateInstanceMeshMaterial()
{
	TArray<UInstancedStaticMeshComponent*> InstMeshComponents = GetAllInstMeshComponents();
	
	for (int ComponentIndex = 0; ComponentIndex < InstMeshComponents.Num(); ++ComponentIndex)
	{
		UInstancedStaticMeshComponent* CurrComponent = InstMeshComponents[ComponentIndex];
		CurrComponent->SetStaticMesh(GetPalette()->Instances[ComponentIndex].Mesh); // Set instance mesh from the current palette index mesh

		for (int MaterialIndex = 0; MaterialIndex < GetPalette()->GetNumMaterialsAtIndex(ComponentIndex); ++MaterialIndex)
		{
			// Set instance material to the current palette index material
			CurrComponent->SetMaterial(MaterialIndex, GetPalette()->Instances[ComponentIndex].Materials[MaterialIndex]);
		}
	}
}

// Delete and Re-adds all instances in each Instanced Static Mesh Component, "updating" each instance with a new set of Transform
void ADecoratorVolumeISM::UpdateInstanceTransform()
{
	const bool UseSurfaceNormal = (Alignment == EInstanceAlignment::SurfaceNormal);
	unsigned int PrevCount = 0; // Sort of a "clamp" for the nested loop
	
	for (int Index = 0; Index < GetAllInstMeshComponents().Num(); ++Index)
	{
		UInstancedStaticMeshComponent* CurrComponent = GetAllInstMeshComponents()[Index];
		
		// Delete all existing instances
		if (CurrComponent->GetInstanceCount() > 0) { CurrComponent->ClearInstances(); }
		
		float CurrCount = FMath::RoundFromZero((Count * (GetPalette()->GetDensityRatioAtIndex(Index))) + PrevCount);
		CurrCount = FMath::Clamp(CurrCount, 0, PointsGeneratorComponent->LineTracedTransforms.Num()); // Clamp value within the number of array elements

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
			FRotator Rotation = UKismetMathLibrary::ComposeRotators(FirstRotation, UseSurfaceNormal ? PointsGeneratorComponent->LineTracedTransforms[j].Rotation : FRotator::ZeroRotator);
			FVector Location = PointsGeneratorComponent->LineTracedTransforms[j].Location;
			FVector Scale = RandomizeScale(Location, ScaleMin, ScaleMax);

			FTransform InstanceTransform = FTransform(Rotation, Location, Scale);
			CurrComponent->AddInstance(InstanceTransform, true);
		}
		
		PrevCount = CurrCount;
	}
}

// Set the correct collision profile to each mesh instance based on the palette instance
void ADecoratorVolumeISM::UpdateInstanceCollisionProfile()
{
	TArray<UInstancedStaticMeshComponent*> ComponentsArray = GetAllInstMeshComponents();
	
	for (int i = 0; i < GetAllInstMeshComponents().Num(); ++i)
	{
		// Setting each instance a collision profile from their respective palette instance
		ComponentsArray[i]->SetCollisionProfileName(GetPalette()->Instances[i].CollisionProfile.Name);
	}
}

UInstancedStaticMeshComponent* ADecoratorVolumeISM::ConstructISMComponent()
{
	return NewObject<UInstancedStaticMeshComponent>(this, UInstancedStaticMeshComponent::StaticClass());
}

UDecoratorPalette* ADecoratorVolumeISM::GetPalette() const
{
	return Palette;
}

TArray<UInstancedStaticMeshComponent*> ADecoratorVolumeISM::GetAllInstMeshComponents()
{
	TArray<UInstancedStaticMeshComponent*> Components;
	GetComponents<UInstancedStaticMeshComponent*>(Components);

	return Components;
}
