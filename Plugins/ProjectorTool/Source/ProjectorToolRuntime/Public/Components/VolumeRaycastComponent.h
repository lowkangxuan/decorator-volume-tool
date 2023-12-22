// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ToolEnums.h"
#include "VolumeRaycastComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTORTOOLRUNTIME_API UVolumeRaycastComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	TArray<FVector> GeneratedPoints;
	TArray<FVector> TracedLocations;
	TArray<FRotator> TracedRotations;
	
public:
	// Sets default values for this component's properties
	UVolumeRaycastComponent();

	UPROPERTY(EditAnywhere)
	EProjectionShape Shape = EProjectionShape::Cylinder;

	// Size for shapes that requires/needs the same X and Y value (e.g. Cube and Cylinder)
	// X is the Length and Breadth, while the Y is the Height
	UPROPERTY(EditAnywhere, meta=(EditCondition = "Shape!=EProjectionShape::Cuboid", EditConditionHides, DisplayName="Size"))
	FVector2D Size2D = FVector2D(200, 100);

	// Size for shapes that can be fully manipulated in any axis (e.g. Cuboid)
	// X is the Length, Y is the Breadth, Z is the Height
	UPROPERTY(EditAnywhere, meta=(EditCondition = "Shape==EProjectionShape::Cuboid", EditConditionHides, DisplayName="Size"))
	FVector Size3D = FVector(200, 200, 100);

	UPROPERTY(EditAnywhere)
	bool Hollow = false;
	
	// Hollow Size for shapes that uses the same X and Y value (e.g. Cube and Cylinder)
	UPROPERTY(EditAnywhere, meta=(EditCondition = "Hollow && Shape!=EProjectionShape::Cuboid", EditConditionHides, DisplayName="Hollow Size"))
	float HollowSizeF = 100;

	// Hollow Size for shapes that have both manipulatable X and Y value (e.g. Cuboid)
	// X is the Length, Y is the Breadth
	UPROPERTY(EditAnywhere, meta=(EditCondition = "Hollow && Shape==EProjectionShape::Cuboid", EditConditionHides, DisplayName="Hollow Size"))
	FVector2D HollowSize2D = FVector2D(100, 100);

	UPROPERTY(EditAnywhere, AdvancedDisplay)
	bool VisualizeRaycast = false;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void RunLineTrace();
};
