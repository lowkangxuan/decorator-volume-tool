// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "InstanceProxyMesh.generated.h"

DECLARE_DELEGATE_OneParam(FOnProxyDestroyedSignature, AInstanceProxyMesh* /* Proxy */)

UCLASS()
class PROJECTORTOOLRUNTIME_API AInstanceProxyMesh : public AActor
{
	GENERATED_BODY()

private:
#if WITH_EDITORONLY_DATA
	UPROPERTY()
	UArrowComponent* ArrowComponent;
#endif

	UPROPERTY(VisibleDefaultsOnly)
	USceneComponent* DefaultRoot;
	
	UPROPERTY(VisibleDefaultsOnly)
	UStaticMeshComponent* StaticMesh;

	bool bSnapEnabled = false;
	
public:
	UPROPERTY(VisibleInstanceOnly)
	int32 ComponentIndex;

	UPROPERTY(VisibleInstanceOnly)
	int32 InstanceIndex;

	FOnProxyDestroyedSignature OnProxyDestroyedDelegate;
	
public:
	// Sets default values for this actor's properties
	AInstanceProxyMesh();

private:
#if WITH_EDITOR
	virtual void EditorApplyTranslation(const FVector& DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;
	virtual void EditorKeyPressed(FKey Key, EInputEvent Event) override;
	
	UFUNCTION(CallInEditor, Category="InstanceProxyMesh")
	void SelectOwnerVolume();
#endif
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetupProxy(UStaticMesh* Mesh, UMaterialInstance* Mat, FRotator BaseRotation, int32 BaseComponentIndex, int32 BaseInstanceIndex);

private:
	void SnapActorDown();
};
