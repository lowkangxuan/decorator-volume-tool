// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ToolEnums.h"
#include "DecoratorVolumeVisualizerComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTORTOOLRUNTIME_API UDecoratorVolumeVisualizerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDecoratorVolumeVisualizerComponent();

private:
	UPROPERTY(VisibleInstanceOnly)
	FVector2D Size = FVector2D::One();

	UPROPERTY(VisibleInstanceOnly)
	FVector CuboidSize = FVector::One();
	
	UPROPERTY(VisibleInstanceOnly)
	EProjectionShape Shape = EProjectionShape::Cylinder;

	UPROPERTY(VisibleInstanceOnly)
	bool bDrawRaycastLines = false;

	UPROPERTY(VisibleInstanceOnly)
	TArray<FVector> RaycastStartPoints = TArray<FVector>();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Getters
	FVector2D GetSize() const { return Size; };
	FVector GetCuboidSize() const { return CuboidSize; };
	EProjectionShape GetShape() const { return Shape; };
	bool CanDrawRaycastLines() const { return bDrawRaycastLines; };
	TArray<FVector> GetPoints() const { return RaycastStartPoints; };

	// Setters
	void UpdateSize(FVector2D NewSize) { Size = NewSize; };
	void UpdateCuboidSize(FVector NewSize) { CuboidSize = CuboidSize; };
	void UpdateShape(EProjectionShape NewShape) { Shape = NewShape; };
	void UpdateStartPoints(TArray<FVector> Points) { RaycastStartPoints = Points; };
	void DrawRaycastLines(bool Flag) { bDrawRaycastLines = Flag; };
};
