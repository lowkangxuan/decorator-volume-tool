// Fill out your copyright notice in the Description page of Project Settings.


#include "TestComponent.h"


// Sets default values for this component's properties
UTestComponent::UTestComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTestComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTestComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTestComponent::AddComponent()
{
	UStaticMeshComponent* MeshComponent = NewObject<UStaticMeshComponent>(GetOwner(), UStaticMeshComponent::StaticClass());

	if (MeshComponent)
	{
		MeshComponent->OnComponentCreated();
		MeshComponent->RegisterComponent();
		MeshComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
		GetOwner()->AddInstanceComponent(MeshComponent);
	}
	UE_LOG(LogTemp, Display, TEXT("%d"), this->GetNumChildrenComponents());
}

