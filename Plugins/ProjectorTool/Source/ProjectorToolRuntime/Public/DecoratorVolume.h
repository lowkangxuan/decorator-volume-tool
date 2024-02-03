// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ToolEnums.h"
#include "DecoratorVolume.generated.h"

class UDecoratorPalette;
class UPointsGeneratorComponent;
class UInstanceBakingComponent;

USTRUCT()
struct FTracedTransform
{
	GENERATED_BODY()

	FVector Location;
	FRotator Rotation;

	FTracedTransform() {};
	FTracedTransform(FVector NewLocation, FRotator NewRotation)
		: Location(NewLocation),
		Rotation(NewRotation)
	{}
};

UCLASS(Abstract, Blueprintable, BlueprintType, HideCategories=(Collision, HLOD, Physics, Networking, Input), meta=(PrioritizeCategories = "DecoratorVolume"))
class PROJECTORTOOLRUNTIME_API ADecoratorVolume : public AActor
{
	GENERATED_BODY()
	
private:
	// Min Max clamps for Seed value
	const int32 MinSeedClamp = -999999;
	const int32 MaxSeedClamp = 999999;

	const uint32 HollowPadding = 100;

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	UBillboardComponent* SpriteComponent;

	UPROPERTY(VisibleAnywhere)
	UInstanceBakingComponent* InstanceBakingComponent = nullptr;
#endif

	UPROPERTY(VisibleAnywhere)
	UPointsGeneratorComponent* PointsGeneratorComponent;
	
	UPROPERTY(VisibleDefaultsOnly)
	USceneComponent* DefaultRoot;

	UPROPERTY()
	FRandomStream RandStream = FRandomStream();
	
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0))
	int32 Count = 0;

	TArray<FVector> GeneratedPoints;
	TArray<FTracedTransform> LineTracedTransforms;
	
public:	
	// Sets default values for this actor's properties
	ADecoratorVolume(const class FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditAnywhere)
	UDecoratorPalette* Palette = nullptr;

	UPROPERTY(EditAnywhere)
	EInstanceAlignment Alignment = EInstanceAlignment::SurfaceNormal;

	UPROPERTY(EditAnywhere, meta = (ClampMin = -999999, ClampMax = 999999))
	int32 Seed;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> ActorsToIgnore;
	
	UPROPERTY(EditAnywhere)
	bool ScaleFromCenter;

	UPROPERTY(EditAnywhere, meta=(EditCondition = "ScaleFromCenter", EditConditionHides))
	EInstanceScaleType ScaleType = EInstanceScaleType::MaxToMin;
	
	UPROPERTY(EditAnywhere, Category="Debug")
	bool DrawRaycastLines = false;

protected:
	// Called when the game starts or when spawneds
	virtual void BeginPlay() override;

#if WITH_EDITOR
	// Function called once a property has been edited in the Details Panel
	virtual void PostEditChangeProperty(FPropertyChangedEvent& e) override;
#endif

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void BeginDestroy() override;

	UFUNCTION(Category="Editor")
	void Regenerate();
	
	UFUNCTION(Category="Editor")
	void GenerateNewSeed();

	UFUNCTION(Category="Editor")
	void Clear();

#if WITH_EDITOR
	UFUNCTION(Category="Editor")
	void BakeInstances();

	UFUNCTION(Category="Editor")
	void UnbakeInstances();
#endif
	
private:
	// Seed Stuff
	void InitNewStreamSeed();
	void RandomizeSeed();

	void TriggerGeneration(bool NewSeed = false);
	void PointsGeneration();
	void RunLineTrace();

	void FlushComponents();
	void UpdateInstanceMeshMaterial();
	void UpdateInstanceTransform();
	void UpdateInstanceCollisionProfile();

	FVector RandomInstanceScale(FVector PointLocation, float ScaleMin, float ScaleMax) const;
	UDecoratorPalette* GetPalette() const;
	TArray<UInstancedStaticMeshComponent*> GetAllInstMeshComponents();
};
