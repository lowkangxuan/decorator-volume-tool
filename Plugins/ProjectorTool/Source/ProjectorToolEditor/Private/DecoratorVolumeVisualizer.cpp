// Fill out your copyright notice in the Description page of Project Settings.


#include "DecoratorVolumeVisualizer.h"
#include "DecoratorVolumeVisualizerComponent.h"

void FDecoratorVolumeVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	const UDecoratorVolumeVisualizerComponent* VisualizerComponent = Cast<UDecoratorVolumeVisualizerComponent>(Component);
	if (!VisualizerComponent) return; // Stop function if component does not exist
	
	const EProjectionShape Shape = VisualizerComponent->GetShape();
	const FVector ActorLocation = VisualizerComponent->GetOwner()->GetActorLocation();
	const FRotationMatrix ActorRotation = FRotationMatrix(VisualizerComponent->GetOwner()->GetActorRotation());
	const FLinearColor Color = FLinearColor::Yellow;
	const float HalfWidth = VisualizerComponent->GetSize().X / 2; // AKA radius
	const float HalfHeight = VisualizerComponent->GetSize().Z / 2;
	constexpr int32 Sides = 32;
	constexpr uint8 DepthPriority = 0;
	constexpr float Thickness = 2;
	constexpr float DepthBias = 0;
	constexpr bool ScreenSpace = false;
	
	switch (Shape)
	{
		case (EProjectionShape::Cylinder):
			{	
				DrawWireCylinder(PDI, ActorLocation, ActorRotation.GetScaledAxis(EAxis::X), ActorRotation.GetScaledAxis(EAxis::Y), ActorRotation.GetScaledAxis(EAxis::Z), Color, HalfWidth, HalfHeight, Sides, DepthPriority, Thickness, DepthBias, ScreenSpace);
				break;
			}

		case (EProjectionShape::Cube):
			{
				const FBox Box = FBox::BuildAABB(FVector::Zero(), FVector(HalfWidth, HalfWidth, HalfHeight));
				const FRotationTranslationMatrix Matrix = FRotationTranslationMatrix(VisualizerComponent->GetOwner()->GetActorRotation(), ActorLocation); // Rotation and Translation Matrix for the box to rotate together with the Actor
				
				DrawWireBox(PDI, Matrix, Box, Color, DepthPriority, Thickness, DepthBias, ScreenSpace);
				break;
			}

		case (EProjectionShape::FreeCube):
			{
				break;
			}
	}

	if (VisualizerComponent->CanDrawRaycastLines())
	{
		TArray<FVector> Points = VisualizerComponent->GetPoints();
		
		for (FVector Point : Points)
		{
			FVector Start = Point + ActorLocation + FVector(0, 0, HalfHeight);
			FMatrix Matrix = FScaleRotationTranslationMatrix(
				FVector::One(),
				VisualizerComponent->GetOwner()->GetActorRotation() - FRotator(90, 0, 0),
				Start
			);

			DrawDirectionalArrow(PDI, Matrix, FLinearColor::Red, VisualizerComponent->GetSize().Z, 5, DepthPriority, Thickness);
		}
	}
}
