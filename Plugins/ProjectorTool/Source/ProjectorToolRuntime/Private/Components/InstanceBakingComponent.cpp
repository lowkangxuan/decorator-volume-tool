// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/InstanceBakingComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "InstanceProxyMesh.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UInstanceBakingComponent::UInstanceBakingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInstanceBakingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInstanceBakingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInstanceBakingComponent::Bake()
{
	// We do not want to run this function it is already baked!!!
	if (bIsBaked) { return; }
	bIsBaked = true;
	
	for (int i = 0; i < UnbakedInstances.Num(); ++i)
	{
		AInstanceProxyMesh* MeshInstance = UnbakedInstances[i];
		FVector LocalLocation = UKismetMathLibrary::InverseTransformLocation(GetOwner()->GetTransform(), MeshInstance->GetActorLocation());
		FTransform NewTransform = FTransform(MeshInstance->GetActorRotation(), LocalLocation, MeshInstance->GetActorScale());
		GetOwnerInstMeshComponents()[MeshInstance->ComponentIndex]->AddInstance(NewTransform);
		MeshInstance->Destroy();
	}

	UnbakedInstances.Empty(); // Clear array
}

void UInstanceBakingComponent::Unbake()
{
	// We do not want to run this function it is not baked!!!
	if (!bIsBaked) { return; }
	
	bIsBaked = false;
	int32 ComponentIndex = 0;
	
	for (UInstancedStaticMeshComponent* Component : GetOwnerInstMeshComponents())
	{
		for (int i = 0; i < Component->GetInstanceCount(); ++i)
		{
			UStaticMesh* Mesh = Component->GetStaticMesh();
			UMaterialInstance* Mat = Cast<UMaterialInstance>(Component->GetMaterial(0));
			FTransform Transform;
			Component->GetInstanceTransform(i, Transform, true);
			ConstructProxyMesh(Mesh, Mat, Transform, ComponentIndex, i);
		}
		++ComponentIndex;
		Component->ClearInstances();
	}
}

void UInstanceBakingComponent::ConstructProxyMesh(UStaticMesh* Mesh, UMaterialInstance* Mat, FTransform Transform, int32 ComponentIndex, int32 InstanceIndex)
{
	AInstanceProxyMesh* ProxyMesh = GetWorld()->SpawnActor<AInstanceProxyMesh>(AInstanceProxyMesh::StaticClass(), Transform);
	ProxyMesh->SetupProxy(Mesh, Mat, Transform.Rotator(), ComponentIndex, InstanceIndex);
	ProxyMesh->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform);
	UnbakedInstances.Add(ProxyMesh);
}

TArray<UInstancedStaticMeshComponent*> UInstanceBakingComponent::GetOwnerInstMeshComponents()
{
	TArray<UInstancedStaticMeshComponent*> Components;
	GetOwner()->GetComponents<UInstancedStaticMeshComponent*>(Components);
	return Components;
}

