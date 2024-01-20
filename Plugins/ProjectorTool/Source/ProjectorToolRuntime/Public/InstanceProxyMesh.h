// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InstanceProxyMesh.generated.h"

UCLASS()
class PROJECTORTOOLRUNTIME_API AInstanceProxyMesh : public AActor
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
	
	UPROPERTY(VisibleInstanceOnly)
	FRotator DefaultRotation = FRotator::ZeroRotator;
	
public:
	// Sets default values for this actor's properties
	AInstanceProxyMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
#if WITH_EDITOR
	virtual void PostEditMove(bool bFinished) override;
	virtual void EditorApplyTranslation(const FVector& DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;
#endif

#if WITH_EDITOR
	UFUNCTION(CallInEditor, Category="InstanceProxyMesh")
	void SelectParentActor();
#endif

	void SetupProxy(UStaticMesh* Mesh, UMaterialInstance* Mat, FRotator BaseRotation, int32 BaseComponentIndex, int32 BaseInstanceIndex);

private:
	void SnapActorDown();
};
