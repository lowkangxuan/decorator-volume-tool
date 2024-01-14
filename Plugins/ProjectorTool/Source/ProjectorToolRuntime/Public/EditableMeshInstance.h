// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EditableMeshInstance.generated.h"

UCLASS()
class PROJECTORTOOLRUNTIME_API AEditableMeshInstance : public AActor
{
	GENERATED_BODY()

private:
#if WITH_EDITORONLY_DATA
	UPROPERTY()
	UBillboardComponent* SpriteComponent;
#endif

	UPROPERTY(VisibleDefaultsOnly)
	USceneComponent* DefaultRoot;
	
	UPROPERTY(VisibleDefaultsOnly)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
	bool bSnapToSurface = true;
	
public:
	UPROPERTY(VisibleInstanceOnly)
	int32 ComponentIndex;

	UPROPERTY(VisibleInstanceOnly)
	int32 InstanceIndex;
	
public:
	// Sets default values for this actor's properties
	AEditableMeshInstance();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostEditMove(bool bFinished) override;

#if WITH_EDITOR
	UFUNCTION(CallInEditor, Category="EditableMeshInstance")
	void SelectParentActor();
#endif

	void SetMesh(UStaticMesh* Mesh);
	void SetMaterial(UMaterialInstance* Mat);

private:
	void SnapActorDown();
};
