// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InstanceBakingComponent.generated.h"

class AInstanceProxyMesh;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTORTOOLRUNTIME_API UInstanceBakingComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	TArray<AInstanceProxyMesh*> UnbakedInstances;

public:
	UPROPERTY(VisibleAnywhere)
	bool bIsBaked = true;

public:
	// Sets default values for this component's properties
	UInstanceBakingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(CallInEditor)
	void Bake();

	UFUNCTION(CallInEditor)
	void Unbake();

private:
	void ConstructProxyMesh(UStaticMesh* Mesh, UMaterialInstance* Mat, FTransform Transform, int32 ComponentIndex, int32 InstanceIndex);
	TArray<UInstancedStaticMeshComponent*> GetOwnerInstMeshComponents();
};
