// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/ToolEnums.h"
#include "PointsGeneratorComponent.generated.h"

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

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTORTOOLRUNTIME_API UPointsGeneratorComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	const uint16 HollowPadding = 100;
	
public:
	// Sets default values for this component's properties
	UPointsGeneratorComponent();

	TArray<FVector> GeneratedPoints;

	UPROPERTY(Transient)
	TArray<FTracedTransform> LineTracedTransforms;

	UPROPERTY(EditAnywhere)
	EInstanceAlignment Alignment = EInstanceAlignment::SurfaceNormal;
	
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
	TArray<AActor*> ActorsToIgnore;
	
	UPROPERTY(EditAnywhere)
	bool Hollow = false;

	// Hollow Size for shapes that uses the same X and Y value (e.g. Cube and Cylinder)
	UPROPERTY(EditAnywhere, meta=(EditCondition = "Hollow && Shape!=EProjectionShape::Cuboid", EditConditionHides, DisplayName="Hollow Size"))
	float HollowSizeF = 100;

	// Hollow Size for shapes that have both manipulatable X and Y value (e.g. Cuboid)
	// X is the Length, Y is the Breadth
	UPROPERTY(EditAnywhere, meta=(EditCondition = "Hollow && Shape==EProjectionShape::Cuboid", EditConditionHides, DisplayName="Hollow Size"))
	FVector2D HollowSize2D = FVector2D(100, 100);

	UPROPERTY(EditAnywhere)
	bool DrawDebugRaycastLines = false;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void GeneratePoints(int32 Count, FRandomStream& Stream);

	UFUNCTION()
	void DoLineTrace();

	FVector GetGenericSize() const;
	FVector GetGenericHollowSize() const;
};
