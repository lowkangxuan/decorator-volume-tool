// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ToolEnums.h"
#include "DecoratorPalette.h"
#include "DecoratorVolumeVisualizerComponent.h"
#include "DecoratorVolume.generated.h"

UCLASS(Abstract, Blueprintable, HideCategories=(Collision, HLOD, Physics, Networking, Input))
class PROJECTORTOOLRUNTIME_API ADecoratorVolume : public AActor
{
	GENERATED_BODY()
	
private:
	// Min Max clamps for Seed value
	const int32 MinClamp = -999999;
	const int32 MaxClamp = 999999;

	bool bIsStreamInitialized = false;
	bool bFlushComponents = false;
	
	UBillboardComponent* SpriteComponent;

	UPROPERTY(VisibleDefaultsOnly)
	USceneComponent* DefaultRoot = nullptr;

	UPROPERTY(VisibleInstanceOnly)
	UDecoratorVolumeVisualizerComponent* VisualizerComponent = nullptr;
	
	FRandomStream RandStream = FRandomStream();
	
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0))
	int32 Count = 0;

	UPROPERTY(VisibleAnywhere)
	TArray<FVector> GeneratedPoints;
	UPROPERTY(VisibleAnywhere)
	TArray<FVector> LineTracedLocations;
	UPROPERTY(VisibleAnywhere)
	TArray<FRotator> LineTracedRotations;
	
public:	
	// Sets default values for this actor's properties
	ADecoratorVolume(const class FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, meta = (DisplayPriority = 0))
	TSubclassOf<UDecoratorPalette> Palette = nullptr;

	UPROPERTY(EditAnywhere, meta = (DisplayPriority = 1))
	EProjectionShape Shape = EProjectionShape::Cylinder;

	UPROPERTY(EditAnywhere, meta=(EditCondition = "Shape!=EProjectionShape::Cuboid", EditConditionHides))
	FVector2D Size = FVector2D(100, 100);

	UPROPERTY(EditAnywhere, meta=(EditCondition = "Shape==EProjectionShape::Cuboid", EditConditionHides, DisplayName="Size"))
	FVector CuboidSize = FVector(100, 100, 100);

	UPROPERTY(EditAnywhere, meta = (ClampMin = -999999, ClampMax = 999999))
	int32 Seed;

	UPROPERTY(EditAnywhere, AdvancedDisplay)
	bool RegenerateOnDetailsEdited = true;
	
	UPROPERTY(EditAnywhere, AdvancedDisplay)
	bool AlignToSurface = true;

	UPROPERTY(EditAnywhere, AdvancedDisplay)
	bool ShowRaycastLines = false;

	UPROPERTY(EditAnywhere, AdvancedDisplay)
	bool ShowInstanceIndex = false;

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
	// Seed Stuff
	void InitNewStreamSeed();
	void RandomizeSeed();
	
	UFUNCTION(CallInEditor, Category="DecoratorVolume")
	void Regenerate();
	
	UFUNCTION(CallInEditor, Category="DecoratorVolume")
	void GenerateNewSeed();

	UFUNCTION(CallInEditor, Category="DecoratorVolume")
	void GenerateNoMesh();

	UFUNCTION(CallInEditor, Category="DecoratorVolume")
	void Clear();
	
	void TriggerGeneration(bool NewSeed = false, bool WithMesh = true);
	void RegeneratePoints();
	void RunLineTrace();
	
	void AddInstMeshComponents();
	void RemoveInstMeshComponents();
	void UpdateInstanceMeshMaterial();
	void UpdateInstanceTransform();

	FRotator RandomizeRotator(FRotator Min, FRotator Max);
	FVector RandomizeScale(FVector Min, FVector Max);

	UDecoratorPalette* GetPalette() const;
	TArray<UInstancedStaticMeshComponent*> GetAllInstMeshComponents() const;
};
