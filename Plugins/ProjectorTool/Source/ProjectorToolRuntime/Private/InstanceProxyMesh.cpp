// Fill out your copyright notice in the Description page of Project Settings.


#include "InstanceProxyMesh.h"
#include "Components/BillboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

#if WITH_EDITOR
	#include "Editor.h"
#endif

// Sets default values
AInstanceProxyMesh::AInstanceProxyMesh()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AActor::SetActorHiddenInGame(true);

#pragma region Setup RootComponent
	DefaultRoot = CreateDefaultSubobject<USceneComponent>("DefaultRoot");
	SetRootComponent(DefaultRoot);
	RootComponent->SetMobility(EComponentMobility::Static);
#pragma endregion

#pragma region Setup Static Mesh
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("InstanceMesh");
	StaticMesh->SetMobility(EComponentMobility::Static);
	StaticMesh->SetupAttachment(RootComponent);
#pragma endregion

#if WITH_EDITORONLY_DATA
	ArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>("Arrow Component");
	if (ArrowComponent)
	{
		ArrowComponent->ArrowColor = FColor::Red;
		ArrowComponent->ArrowSize = 1.0f;
		ArrowComponent->bTreatAsASprite = true;
		ArrowComponent->SetupAttachment(RootComponent);
		ArrowComponent->bIsScreenSizeScaled = true;
	}
	
#endif
}

// Called when the game starts or when spawned
void AInstanceProxyMesh::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInstanceProxyMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#if WITH_EDITOR
void AInstanceProxyMesh::EditorApplyTranslation(const FVector& DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown)
{
	Super::EditorApplyTranslation(DeltaTranslation, bAltDown, bShiftDown, bCtrlDown);

	if (bSnapEnabled) { SnapActorDown(); }
}

void AInstanceProxyMesh::EditorKeyPressed(FKey Key, EInputEvent Event)
{
	Super::EditorKeyPressed(Key, Event);

	if (Key.GetDisplayName().ToString() == "s")
	{
		if (Event == IE_Pressed) { bSnapEnabled = true; }
		else if (Event == IE_Released) { bSnapEnabled = false; }
	}
}

void AInstanceProxyMesh::SelectOwnerVolume()
{
	// Deselects self and selects the owning Decorator Volume actor
	GEditor->SelectActor(this, false, true);
	GEditor->SelectActor(GetAttachParentActor(), true, true);
}
#endif

void AInstanceProxyMesh::SetupProxy(UStaticMesh* Mesh, UMaterialInstance* Mat, FRotator BaseRotation, int32 BaseComponentIndex, int32 BaseInstanceIndex)
{
	StaticMesh->SetStaticMesh(Mesh);
	StaticMesh->SetMaterial(0, Mat);
	this->ComponentIndex = BaseComponentIndex;
	this->InstanceIndex = BaseInstanceIndex;
}

void AInstanceProxyMesh::SnapActorDown()
{
	FHitResult OutHit;
	FCollisionQueryParams TraceParams;
	FVector Start = GetActorLocation() + FVector(0, 0, 100);
	FVector End = GetActorLocation() - FVector(0, 0, 500);
	TraceParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_WorldStatic, TraceParams);

	if (OutHit.bBlockingHit)
	{
		SetActorLocation(OutHit.ImpactPoint);
		FRotator HitRotation = UKismetMathLibrary::MakeRotFromZ(OutHit.ImpactNormal);
		FRotator FinalRotation = UKismetMathLibrary::ComposeRotators(FRotator(0, 90, 0), HitRotation);
		SetActorRotation(FinalRotation);
	}
}

