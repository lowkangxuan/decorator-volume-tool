// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ToolEnums.h"
#include "DecoratorPalette.h"
#include "DecoratorVolumeVisualizerComponent.h"
#include "DecoratorVolume.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType, HideCategories=(Collision, HLOD, Physics, Networking, Input))
class PROJECTORTOOLRUNTIME_API ADecoratorVolume : public AActor
{
	GENERATED_BODY()
	
private:
	// Min Max clamps for Seed value
	const int32 MinSeedClamp = -999999;
	const int32 MaxSeedClamp = 999999;

	const uint32 HollowPadding = 100;
	
	bool bIsStreamInitialized = false;

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	UBillboardComponent* SpriteComponent;
#endif

	UPROPERTY()
	UDecoratorVolumeVisualizerComponent* VisualizerComponent;
	
	UPROPERTY(VisibleDefaultsOnly)
	USceneComponent* DefaultRoot;

	UPROPERTY()
	FRandomStream RandStream = FRandomStream();
	
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0))
	int32 Count = 0;

	TArray<FVector> GeneratedPoints;
	TArray<FVector> LineTracedLocations;
	TArray<FRotator> LineTracedRotations;
	
public:	
	// Sets default values for this actor's properties
	ADecoratorVolume(const class FObjectInitializer& ObjectInitializer);

	
	UPROPERTY(EditAnywhere)
	UDecoratorPalette* Palette = nullptr;

	UPROPERTY(EditAnywhere)
	EProjectionShape Shape = EProjectionShape::Cylinder;

	UPROPERTY(EditAnywhere)
	EInstanceAlignment Alignment = EInstanceAlignment::SurfaceNormal;

	// Size for shapes that requires/needs the same X and Y value (e.g. Cube and Cylinder)
	// X is the Length and Breadth, while the Y is the Height
	UPROPERTY(EditAnywhere, meta=(EditCondition = "Shape!=EProjectionShape::Cuboid", EditConditionHides, DisplayName="Size"))
	FVector2D Size2D = FVector2D(200, 100);

	// Size for shapes that can be fully manipulated in any axis (e.g. Cuboid)
	// X is the Length, Y is the Breadth, Z is the Height
	UPROPERTY(EditAnywhere, meta=(EditCondition = "Shape==EProjectionShape::Cuboid", EditConditionHides, DisplayName="Size"))
	FVector Size3D = FVector(200, 200, 100);

	UPROPERTY(EditAnywhere, meta = (ClampMin = -999999, ClampMax = 999999))
	int32 Seed;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> ActorsToIgnore;
	
	UPROPERTY(EditAnywhere)
	bool ScaleFromCenter;

	UPROPERTY(EditAnywhere, meta=(EditCondition = "ScaleFromCenter", EditConditionHides))
	EInstanceScaleType ScaleType = EInstanceScaleType::MaxToMin;
	
	UPROPERTY(EditAnywhere)
	bool Hollow = false;
	
	// Hollow Size for shapes that uses the same X and Y value (e.g. Cube and Cylinder)
	UPROPERTY(EditAnywhere, meta=(EditCondition = "Hollow && Shape!=EProjectionShape::Cuboid", EditConditionHides, DisplayName="Hollow Size"))
	float HollowSizeF = 100;

	// Hollow Size for shapes that have both manipulatable X and Y value (e.g. Cuboid)
	// X is the Length, Y is the Breadth
	UPROPERTY(EditAnywhere, meta=(EditCondition = "Hollow && Shape==EProjectionShape::Cuboid", EditConditionHides, DisplayName="Hollow Size"))
	FVector2D HollowSize2D = FVector2D(100, 100);
	
	UPROPERTY(EditAnywhere, Category="Debug")
	bool DrawRaycastLines = false;

	UPROPERTY(EditAnywhere, Category="Debug")
	bool ShowInstanceIndex = false;

protected:
	// Called when the game starts or when spawneds
	virtual void BeginPlay() override;

#if WITH_EDITOR
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
	// Seed Stuff
	void InitNewStreamSeed();
	void RandomizeSeed();
	
	UFUNCTION(CallInEditor, Category="DecoratorVolume")
	void Regenerate();
	
	UFUNCTION(CallInEditor, Category="DecoratorVolume")
	void GenerateNewSeed();

	UFUNCTION(CallInEditor, Category="DecoratorVolume")
	void Clear();
	
	void TriggerGeneration(bool NewSeed = false);
	void PointsGeneration();
	void RunLineTrace();

	void FlushComponents();
	void UpdateInstanceMeshMaterial();
	void UpdateInstanceTransform();
	void UpdateInstanceCollisionProfile();

	FVector RandomInstanceScale(FVector PointLocation, float ScaleMin, float ScaleMax) const;
	UDecoratorPalette* GetPalette() const;
	FVector GetGenericSize() const;
	TArray<UInstancedStaticMeshComponent*> GetAllInstMeshComponents() const;
};
