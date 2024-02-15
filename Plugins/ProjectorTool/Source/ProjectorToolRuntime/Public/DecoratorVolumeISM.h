// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseDecoratorVolume.h"
#include "GameFramework/Actor.h"
#include "DecoratorVolumeISM.generated.h"

class ABaseDecoratorVolume;
class UDecoratorPalette;
class UPointsGeneratorComponent;
class UInstanceBakingComponent;

/**
 * Decorator Volume that stores instances in an Instanced Static Mesh Component
 */
UCLASS(Abstract, HideCategories=(Collision, HLOD, Physics, Networking, Input))
class PROJECTORTOOLRUNTIME_API ADecoratorVolumeISM : public ABaseDecoratorVolume
{
	GENERATED_BODY()
	
private:
	// Min Max clamps for Seed value
	const int32 MinSeedClamp = -999999;
	const int32 MaxSeedClamp = 999999;

#if WITH_EDITORONLY_DATA

	UPROPERTY(VisibleAnywhere)
	UInstanceBakingComponent* InstanceBakingComponent = nullptr;
#endif
	
public:	
	// Sets default values for this actor's properties
	ADecoratorVolumeISM(const class FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditAnywhere)
	UDecoratorPalette* Palette = nullptr;
	bool bIsCleared = false;

protected:
	// Called when the game starts or when spawneds
	virtual void BeginPlay() override;

#if WITH_EDITOR
	// Function called once a property has been edited in the Details Panel
	virtual void PostEditChangeProperty(FPropertyChangedEvent& e) override;
#endif

	virtual void TriggerGeneration(bool NewSeed = false) override;
	virtual UInstancedStaticMeshComponent* ConstructISMComponent();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Clear() override;

#if WITH_EDITOR
	UFUNCTION(Category="Editor")
	void BakeInstances();

	UFUNCTION(Category="Editor")
	void UnbakeInstances();
#endif
	
private:

	void FlushComponents();
	void UpdateInstanceMeshMaterial();
	void UpdateInstanceTransform();
	void UpdateInstanceCollisionProfile();

	UDecoratorPalette* GetPalette() const;
	TArray<UInstancedStaticMeshComponent*> GetAllInstMeshComponents();
};
