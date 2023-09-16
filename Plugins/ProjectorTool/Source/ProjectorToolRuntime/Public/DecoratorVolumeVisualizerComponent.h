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
	FVector Size = FVector::One();
	
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
	FVector GetSize() const;
	EProjectionShape GetShape() const;
	bool CanDrawRaycastLines() const;
	TArray<FVector> GetPoints() const;

	// Setters
	void UpdateSize(FVector NewSize);
	void UpdateShape(EProjectionShape Shape);
	void UpdateStartPoints(TArray<FVector> Points);
	void DrawRaycastLines(bool Flag);
};
