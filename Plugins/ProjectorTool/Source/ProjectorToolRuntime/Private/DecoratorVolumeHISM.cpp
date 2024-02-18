// Fill out your copyright notice in the Description page of Project Settings.


#include "DecoratorVolumeHISM.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"

// Sets default values
ADecoratorVolumeHISM::ADecoratorVolumeHISM(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADecoratorVolumeHISM::BeginPlay()
{
	Super::BeginPlay();
	
}

UInstancedStaticMeshComponent* ADecoratorVolumeHISM::ConstructISMComponent()
{
	return NewObject<UHierarchicalInstancedStaticMeshComponent>(this, UHierarchicalInstancedStaticMeshComponent::StaticClass());
}

// Called every frame
void ADecoratorVolumeHISM::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

