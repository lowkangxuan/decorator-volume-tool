// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ToolEnums.h"
#include "BaseDecoratorVolume.generated.h"

class UBillboardComponent;
class UPointsGeneratorComponent;

UCLASS(Abstract, BlueprintType)
class PROJECTORTOOLRUNTIME_API ABaseDecoratorVolume : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly)
	USceneComponent* DefaultRoot;

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	UBillboardComponent* SpriteComponent;
#endif
	
protected:
	// Min Max clamps for Seed value
	const int32 MinSeedClamp = -999999;
	const int32 MaxSeedClamp = 999999;

	UPROPERTY(VisibleAnywhere)
	UPointsGeneratorComponent* PointsGeneratorComponent;
	
	UPROPERTY()
	FRandomStream RandStream = FRandomStream();

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0))
	int32 Count = 0;
	
public:
	// Sets default values for this actor's properties
	ABaseDecoratorVolume(const class FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere)
	EInstanceAlignment Alignment = EInstanceAlignment::SurfaceNormal;

	UPROPERTY(EditAnywhere, meta = (ClampMin = -999999, ClampMax = 999999))
	int32 Seed;
	
	UPROPERTY(EditAnywhere)
	bool ScaleFromCenter;

	UPROPERTY(EditAnywhere, meta=(EditCondition = "ScaleFromCenter", EditConditionHides))
	EInstanceScaleType ScaleType = EInstanceScaleType::MaxToMin;
	
	UPROPERTY(EditAnywhere, Category="Debug")
	bool DrawRaycastLines = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#if WITH_EDITOR
	// Function called once a property has been edited in the Details Panel
	virtual void PostEditChangeProperty(FPropertyChangedEvent& e) override;
#endif

	// Seed Stuff
	void InitNewStreamSeed();
	void RandomizeSeed();

	virtual void TriggerGeneration(bool NewSeed = false);
	void PointsGeneration();
	
	FVector RandomizeScale(FVector PointLocation, float ScaleMin, float ScaleMax) const;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Category="Editor")
	virtual void Regenerate();
	
	UFUNCTION(Category="Editor")
	virtual void GenerateNewSeed();

	UFUNCTION(Category="Editor")
	virtual void Clear();
};
