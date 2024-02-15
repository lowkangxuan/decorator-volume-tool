// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DecoratorVolumeISM.h"
#include "DecoratorVolumeHISM.generated.h"

/**
 * Decorator Volume that stores instances in an Hierarchical Instanced Static Mesh Component
 */
UCLASS()
class PROJECTORTOOLRUNTIME_API ADecoratorVolumeHISM : public ADecoratorVolumeISM
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADecoratorVolumeHISM(const class FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual UInstancedStaticMeshComponent* ConstructISMComponent() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
