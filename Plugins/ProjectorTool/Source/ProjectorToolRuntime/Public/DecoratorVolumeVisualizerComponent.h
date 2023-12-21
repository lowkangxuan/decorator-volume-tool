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
	FVector2D Size2D = FVector2D::One();

	UPROPERTY(VisibleInstanceOnly)
	FVector Size3D = FVector::One();
	
	UPROPERTY(VisibleInstanceOnly)
	float HollowSizeF = 100;

	UPROPERTY(VisibleInstanceOnly)
	FVector2D HollowSize2D = FVector2D(100, 100);
	
	UPROPERTY(VisibleInstanceOnly)
	EProjectionShape Shape = EProjectionShape::Cylinder;

	UPROPERTY(VisibleInstanceOnly)
	bool bDrawRaycastLines = false;

	UPROPERTY(VisibleInstanceOnly)
	bool bDrawHollow = false;

	UPROPERTY(VisibleInstanceOnly)
	TArray<FVector> RaycastStartPoints = TArray<FVector>();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Getters
	FVector2D GetSize2D() const { return Size2D; }
	FVector GetSize3D() const { return Size3D; }
	float GetHollowSizeF() const { return HollowSizeF; }
	FVector2d GetHollowSize2D() const { return HollowSize2D; }
	
	EProjectionShape GetShape() const { return Shape; }
	bool CanDrawHollow() const { return bDrawHollow; }
	bool CanDrawRaycastLines() const { return bDrawRaycastLines; }
	TArray<FVector> GetPoints() const { return RaycastStartPoints; }

	// Setters
	void UpdateSize2D(FVector2D NewSize) { Size2D = NewSize; }
	void UpdateSize3D(FVector NewSize) { Size3D = NewSize; }
	void UpdateHollowSizeF(float NewSize) { HollowSizeF = NewSize; }
	void UpdateHollowSize2D(FVector2D NewSize) { HollowSize2D = NewSize; }
	
	void UpdateShape(EProjectionShape NewShape) { Shape = NewShape; }
	void DrawHollow(bool Flag) { bDrawHollow = Flag; }
	void DrawRaycastLines(bool Flag) { bDrawRaycastLines = Flag; }
	void UpdateStartPoints(TArray<FVector> Points) { RaycastStartPoints = Points; }
};
