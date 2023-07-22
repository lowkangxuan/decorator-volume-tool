// Fill out your copyright notice in the Description page of Project Settings.


#include "MySceneComponent.h"

#include "Components/InstancedStaticMeshComponent.h"

// Sets default values for this component's properties
UMySceneComponent::UMySceneComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMySceneComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMySceneComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMySceneComponent::TestButton()
{
	UInstancedStaticMeshComponent* InstMeshComp = NewObject<UInstancedStaticMeshComponent>(this, TEXT("TestComponent"));
	if (!InstMeshComp) return; // Exit the function if for some reason the component is not created
		
	InstMeshComp->OnComponentCreated();
	InstMeshComp->CreationMethod = EComponentCreationMethod::Instance;
	InstMeshComp->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	InstMeshComp->SetRelativeLocation(FVector(0)); // Ensure that the component is at the center of the actor
	//InstMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InstMeshComp->RegisterComponent();
	GetOwner()->AddInstanceComponent(InstMeshComp);
}

