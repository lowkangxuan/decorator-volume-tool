// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/InstanceBakingComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Engine/StaticMeshActor.h"
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
	
	while (UnbakedProxies.Num() > 0)
	{
		AInstanceProxyMesh* ProxyMesh = UnbakedProxies[0];
		
		//FVector LocalLocation = UKismetMathLibrary::InverseTransformLocation(GetOwner()->GetTransform(), ProxyMesh->GetActorLocation());
		FTransform NewTransform = FTransform(ProxyMesh->GetActorRotation(), ProxyMesh->GetActorLocation() - GetOwner()->GetActorLocation(), ProxyMesh->GetActorScale());
		GetOwnerInstMeshComponents()[ProxyMesh->ComponentIndex]->AddInstance(NewTransform);
		ProxyMesh->Destroy();
	}
	
	Modify();
}

void UInstanceBakingComponent::Unbake()
{
	// We do not want to run this function when it is not baked!!!
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
		Component->Modify();
	}
}

void UInstanceBakingComponent::ConstructProxyMesh(UStaticMesh* Mesh, UMaterialInstance* Mat, FTransform Transform, int32 ComponentIndex, int32 InstanceIndex)
{
	AInstanceProxyMesh* ProxyMesh = GetWorld()->SpawnActor<AInstanceProxyMesh>(AInstanceProxyMesh::StaticClass(), Transform);
	ProxyMesh->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform);
	ProxyMesh->SetupProxy(Mesh, Mat, Transform.Rotator(), ComponentIndex, InstanceIndex);
	ProxyMesh->OnProxyDestroyedDelegate.BindUObject(this, &ThisClass::RemoveProxy);
	UnbakedProxies.Add(ProxyMesh);
}

void UInstanceBakingComponent::RemoveProxy(AInstanceProxyMesh* DestroyedProxy)
{
	UnbakedProxies.RemoveSingle(DestroyedProxy);
	DestroyedProxy->OnProxyDestroyedDelegate.Unbind();
}

TArray<UInstancedStaticMeshComponent*> UInstanceBakingComponent::GetOwnerInstMeshComponents()
{
	TArray<UInstancedStaticMeshComponent*> Components;
	GetOwner()->GetComponents<UInstancedStaticMeshComponent*>(Components);
	return Components;
}

