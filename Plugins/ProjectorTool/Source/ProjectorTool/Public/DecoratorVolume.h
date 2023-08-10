// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DecoratorPalette.h"
#include "DecoratorVolume.generated.h"

UENUM()
enum ProjectionShape
{
	Cylinder,
	Cube
};

UCLASS(Abstract, Blueprintable, HideCategories=(Collision, HLOD, Physics, Networking, Input))
class PROJECTORTOOL_API ADecoratorVolume : public AActor
{
	GENERATED_BODY()
	
private:
	// Min Max clamps for Seed value
	const int32 MinClamp = -999999;
	const int32 MaxClamp = 999999;
	
	UBillboardComponent* SpriteComponent;
	UStaticMesh* DebugCylinderMesh;
	UStaticMesh* DebugCubeMesh;
	UMaterialInterface* DebugMeshMat;

	UPROPERTY(VisibleDefaultsOnly)
	USceneComponent* DefaultRoot = nullptr;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* DebugMesh = nullptr;

	TArray<FVector> GeneratedPoints;
	TArray<FVector> LineTracedLocations;
	TArray<FRotator> LineTracedRotations;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0))
	int32 Count = 0;
	FRandomStream RandStream = FRandomStream();
	
public:	
	// Sets default values for this actor's properties
	ADecoratorVolume(const class FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, meta = (DisplayPriority = 0))
	TSubclassOf<UDecoratorPalette> Palette = nullptr;

	UPROPERTY(EditAnywhere, meta = (DisplayPriority = 1))
	TEnumAsByte<ProjectionShape> Shape = ProjectionShape::Cylinder;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FVector2D Size = FVector2D(100, 100);

	UPROPERTY(EditAnywhere, meta = (ClampMin = -999999, ClampMax = 999999))
	int32 Seed = 0;

	UPROPERTY(EditAnywhere, AdvancedDisplay)
	bool AlignToSurface = true;

	UPROPERTY(EditAnywhere, AdvancedDisplay)
	bool DrawProjectionVisualizer = true;

protected:
	// Called when the game starts or when spawneds
	virtual void BeginPlay() override;

#if WITH_EDITOR
	// Function called once properties are first initialized
	virtual void PostInitProperties() override;
	
	// Function called once a property has been edited in the Details Panel
	virtual void PostEditChangeProperty(FPropertyChangedEvent& e) override;
#endif

public:	
	// Function similar to the Construction Script
	virtual void OnConstruction(const FTransform& Transform) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void BeginDestroy() override;

private:
	UFUNCTION(CallInEditor, Category="DecoratorVolume")
	void Regenerate();
	void TriggerRegeneration(bool NewSeed);
	void RegeneratePoints();
	void RunLineTrace();
	
	UFUNCTION(CallInEditor, Category="DecoratorVolume")
	void RepaintTransform();
	
	UFUNCTION(CallInEditor, Category="DecoratorVolume")
	void RepaintMeshMaterial();
	
	void AddInstMeshComponents();
	void DeleteInstMeshComponents();
	void UpdateInstanceMeshMaterial();
	void UpdateInstanceTransform();
	
	void UpdateMeshScale();

	// Seed Stuff
	void InitNewStreamSeed();
	void RandomizeSeed();

	// Overloads for DrawDebugLines
	void DrawDebugLines();
	void DrawDebugLines(FVector Start, FVector End);

	UDecoratorPalette* GetPalette() const;
};
