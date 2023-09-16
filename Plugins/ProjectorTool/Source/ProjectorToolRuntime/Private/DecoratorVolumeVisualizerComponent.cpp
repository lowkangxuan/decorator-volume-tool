// Fill out your copyright notice in the Description page of Project Settings.


#include "DecoratorVolumeVisualizerComponent.h"


// Sets default values for this component's properties
UDecoratorVolumeVisualizerComponent::UDecoratorVolumeVisualizerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UDecoratorVolumeVisualizerComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UDecoratorVolumeVisualizerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

FVector UDecoratorVolumeVisualizerComponent::GetSize() const
{
	return Size;
}

EProjectionShape UDecoratorVolumeVisualizerComponent::GetShape() const
{
	return Shape;
}

bool UDecoratorVolumeVisualizerComponent::CanDrawRaycastLines() const
{
	return bDrawRaycastLines;
}

TArray<FVector> UDecoratorVolumeVisualizerComponent::GetPoints() const
{
	return RaycastStartPoints;
}

void UDecoratorVolumeVisualizerComponent::DrawRaycastLines(bool Flag)
{
	bDrawRaycastLines = Flag;
}

void UDecoratorVolumeVisualizerComponent::UpdateSize(FVector NewSize)
{
	Size = NewSize;
}

void UDecoratorVolumeVisualizerComponent::UpdateShape(EProjectionShape NewShape)
{
	Shape = NewShape;
}

void UDecoratorVolumeVisualizerComponent::UpdateStartPoints(TArray<FVector> Points)
{
	RaycastStartPoints = Points;
}

