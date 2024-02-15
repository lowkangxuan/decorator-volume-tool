// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseDecoratorVolume.h"
#include "Components/BillboardComponent.h"
#include "Components/PointsGeneratorComponent.h"

// Sets default values
ABaseDecoratorVolume::ABaseDecoratorVolume(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

#pragma region Setting RootComponent
	// Setting up SceneComponent as default RootComponent
	DefaultRoot = CreateDefaultSubobject<USceneComponent>("DefaultRoot");
	SetRootComponent(DefaultRoot);
	RootComponent->SetMobility(EComponentMobility::Static);
#pragma endregion Setting RootComponent

PointsGeneratorComponent = CreateDefaultSubobject<UPointsGeneratorComponent>("Points Generator Component");

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
void ABaseDecoratorVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseDecoratorVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#if WITH_EDITOR
void ABaseDecoratorVolume::PostEditChangeProperty(FPropertyChangedEvent& e)
{
	Super::PostEditChangeProperty(e);
	const FName PropertyName = (e.Property != NULL ? e.MemberProperty->GetFName() : NAME_None);

	// Generate new points when these properties are edited
	if (PropertyName == "Seed")
	{
		InitNewStreamSeed();
		TriggerGeneration();
	}

	if (PropertyName == "Count" || PropertyName == "ScaleFromCenter" || PropertyName == "ScaleType" || PropertyName == "Alignment")
	{
		TriggerGeneration();
	}
}
#endif

void ABaseDecoratorVolume::InitNewStreamSeed()
{
	RandStream.Initialize(Seed);
}

void ABaseDecoratorVolume::RandomizeSeed()
{
	Seed = FMath::RandRange(MinSeedClamp, MaxSeedClamp);
	InitNewStreamSeed();
}

void ABaseDecoratorVolume::Regenerate()
{
	TriggerGeneration();
}

void ABaseDecoratorVolume::GenerateNewSeed()
{
	TriggerGeneration(true);
}

void ABaseDecoratorVolume::Clear()
{
}

void ABaseDecoratorVolume::TriggerGeneration(bool NewSeed)
{
	if (Count == 0) return;
	if (NewSeed) RandomizeSeed(); // Randomize a new seed if NewSeed param is true

	RandStream.Reset(); // Resets the Stream to get back original RANDOM values
	PointsGeneration();
	Modify(); // Marks the actor as dirty and require saving
}

void ABaseDecoratorVolume::PointsGeneration()
{
	PointsGeneratorComponent->GeneratePoints(Count, RandStream);
	PointsGeneratorComponent->DoLineTrace();
}

// Generate random scale from desired Min Max value
FVector ABaseDecoratorVolume::RandomizeScale(FVector PointLocation, float ScaleMin, float ScaleMax) const
{
	if (ScaleFromCenter)
	{
		// Lerping the scale based on the distance of the point from (0, 0, 0) divided by the size of the volume
		const float DistFromCenter = FMath::Abs(FVector::Distance(FVector::Zero(), PointLocation - GetActorLocation()));
		const float DesiredMin = (ScaleType == EInstanceScaleType::MinToMax ? ScaleMin : ScaleMax);
		const float DesiredMax = (ScaleType == EInstanceScaleType::MinToMax ? ScaleMax : ScaleMin);

		return FVector(FMath::Lerp(DesiredMin, DesiredMax, DistFromCenter/(PointsGeneratorComponent->GetGenericSize().X/2)));
	}

	return FVector(RandStream.FRandRange(ScaleMin, ScaleMax));
}

