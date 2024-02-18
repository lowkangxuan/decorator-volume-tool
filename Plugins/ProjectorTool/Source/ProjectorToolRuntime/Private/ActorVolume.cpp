// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorVolume.h"


// Sets default values
AActorVolume::AActorVolume(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AActorVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActorVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

