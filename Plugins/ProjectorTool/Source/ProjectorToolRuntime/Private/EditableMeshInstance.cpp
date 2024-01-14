// Fill out your copyright notice in the Description page of Project Settings.


#include "EditableMeshInstance.h"
#include "Components/BillboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

#if WITH_EDITOR
	#include "Editor.h"
#endif

// Sets default values
AEditableMeshInstance::AEditableMeshInstance()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

#pragma region Setup RootComponent
	DefaultRoot = CreateDefaultSubobject<USceneComponent>("DefaultRoot");
	SetRootComponent(DefaultRoot);
	RootComponent->SetMobility(EComponentMobility::Static);
#pragma endregion

#pragma region Setup Static Mesh
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("InstanceMesh");
	StaticMesh->SetMobility(EComponentMobility::Static);
	StaticMesh->SetupAttachment(RootComponent);
#pragma endregion
	
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
void AEditableMeshInstance::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEditableMeshInstance::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEditableMeshInstance::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	if (bFinished && bSnapToSurface)
	{
		SnapActorDown();
	}
}

#if WITH_EDITOR
void AEditableMeshInstance::SelectParentActor()
{
	// Deselects self and selects the owning Decorator Volume actor
	GEditor->SelectActor(this, false, true);
	GEditor->SelectActor(GetAttachParentActor(), true, true);
}
#endif

void AEditableMeshInstance::SetMesh(UStaticMesh* Mesh)
{
	StaticMesh->SetStaticMesh(Mesh);
}

void AEditableMeshInstance::SetMaterial(UMaterialInstance* Mat)
{
	StaticMesh->SetMaterial(0, Mat);
}

void AEditableMeshInstance::SnapActorDown()
{
	FHitResult OutHit;
	FVector Start = GetActorLocation() + FVector(0, 0, 10);
	FVector End = GetActorLocation() - FVector(0, 0, 100);
	GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_WorldStatic);

	if (OutHit.bBlockingHit)
	{
		SetActorLocation(OutHit.ImpactPoint);
		FRotator FirstRotation = FRotator(0, 90, 0);
		FRotator HitRotation = UKismetMathLibrary::MakeRotFromZ(OutHit.ImpactNormal);
		FRotator FinalRotation = UKismetMathLibrary::ComposeRotators(FirstRotation, HitRotation);
		SetActorRotation(FinalRotation);
	}
}

