// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseDecoratorVolume.h"
#include "ActorVolume.generated.h"

UCLASS()
class PROJECTORTOOLRUNTIME_API AActorVolume : public ABaseDecoratorVolume
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AActorVolume(const class FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
