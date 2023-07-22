// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DecoratorPalette.h"
#include "DecoratorVolumeComponent.generated.h"

UENUM()
enum TestVolumeShape
{
	Cylinder,
	Cube
};

UCLASS( ClassGroup=(Custom), HideCategories("Activation", "Collision", "Tags","AssetUserData"),meta=(BlueprintSpawnableComponent) )
class PROPPROJECTORTOOL_API UDecoratorVolumeComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDecoratorVolumeComponent();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced, meta = (DisplayPriority = 0))
	UDecoratorPalette* Palette = nullptr;

	UPROPERTY(EditAnywhere, meta = (DisplayPriority = 1))
	TEnumAsByte<TestVolumeShape> Shape = TestVolumeShape::Cylinder;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FVector2D Size = FVector2D(100, 100);

	UPROPERTY(EditAnywhere, meta = (ClampMin = -999999, ClampMax = 999999))
	int32 Seed = 0;
	
	UPROPERTY(BlueprintReadOnly)
	FRandomStream RandStream = FRandomStream();
	
private:
	// Min Max clamps for Seed value
	const int32 MinClamp = -999999;
	const int32 MaxClamp = 999999;

	UPROPERTY(EditAnywhere/*, meta = (MakeEditWidget)*/)
	TArray<FVector> GeneratedPoints;
	TArray<FVector> LineTracedLocations;
	TArray<FVector> LineTracedRotations;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0))
	int32 Count = 0;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

#if WITH_EDITOR
	// Function called once properties are first initialized
	virtual void PostInitProperties() override;
	
	// Function called once a property has been edited in the Details Panel
	virtual void PostEditChangeProperty(FPropertyChangedEvent& e) override;
	void OnSelectionChanged(UObject* NewSelection);
#endif
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UFUNCTION(CallInEditor, Category="DecoratorVolume")
	void Regenerate();
	void TriggerRegeneration(bool NewSeed);
	void RegeneratePoints();
	
	void AddInstMeshComps();
	void DeleteInstMeshComps();
	void UpdateInstMeshComps();
	UFUNCTION(CallInEditor, Category="DecoratorVolume")
	void RunLineTraces();
	
	void UpdateMeshScale();

	void InitNewStreamSeed();
	void RandomizeSeed();

	// Overloads for DrawDebugLines
	void DrawDebugLines();
	void DrawDebugLines(FVector Start, FVector End);
};
