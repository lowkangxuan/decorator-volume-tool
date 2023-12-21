// Fill out your copyright notice in the Description page of Project Settings.


#include "DecoratorVolumeVisualizer.h"
#include "DecoratorVolumeVisualizerComponent.h"

// Rewrote this function for Unreal's DrawDirectionalArrow function (https://github.com/EpicGames/UnrealEngine/blob/a3cb3d8fdec1fc32f071ae7d22250f33f80b21c4/Engine/Source/Runtime/Engine/Private/PrimitiveDrawingUtils.cpp#L1385)
// As the Unreal's version defaults to X axis, this function will default it to the -Z axis for the tool usecase
void FDecoratorVolumeVisualizer::DrawDirectionalArrowNegZ(FPrimitiveDrawInterface* PDI,const FMatrix& ArrowToWorld,const FLinearColor& InColor,float Length,float ArrowSize,uint8 DepthPriority,float Thickness)
{
	PDI->DrawLine(ArrowToWorld.TransformPosition(FVector(0,0,-Length)),ArrowToWorld.TransformPosition(FVector::ZeroVector),InColor,DepthPriority,Thickness);
	PDI->DrawLine(ArrowToWorld.TransformPosition(FVector(0,0,-Length)),ArrowToWorld.TransformPosition(FVector(+ArrowSize,         0,-Length+ArrowSize)), InColor, DepthPriority, Thickness); // Front
	PDI->DrawLine(ArrowToWorld.TransformPosition(FVector(0,0,-Length)),ArrowToWorld.TransformPosition(FVector(-ArrowSize,         0,-Length+ArrowSize)), InColor, DepthPriority, Thickness); // Back
	PDI->DrawLine(ArrowToWorld.TransformPosition(FVector(0,0,-Length)),ArrowToWorld.TransformPosition(FVector(		  0,+ArrowSize,-Length+ArrowSize)), InColor, DepthPriority, Thickness); // Right
	PDI->DrawLine(ArrowToWorld.TransformPosition(FVector(0,0,-Length)),ArrowToWorld.TransformPosition(FVector(		  0,-ArrowSize,-Length+ArrowSize)), InColor, DepthPriority, Thickness); // Left
}

void FDecoratorVolumeVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	const UDecoratorVolumeVisualizerComponent* VisualizerComponent = Cast<UDecoratorVolumeVisualizerComponent>(Component);
	if (!VisualizerComponent) return; // Stop function if component does not exist
	
	const EProjectionShape Shape = VisualizerComponent->GetShape();
	const FVector ActorLocation = VisualizerComponent->GetOwner()->GetActorLocation();
	const FRotator ActorRotation = VisualizerComponent->GetOwner()->GetActorRotation();
	const FRotationMatrix ActorRotationMatrix = FRotationMatrix(VisualizerComponent->GetOwner()->GetActorRotation());
	const FLinearColor Color = FLinearColor::Yellow;
	const float MaxHeight = (Shape == EProjectionShape::Cuboid ? VisualizerComponent->GetSize3D().Z : VisualizerComponent->GetSize2D().Y);
	const float HalfHeight = (Shape == EProjectionShape::Cuboid ? VisualizerComponent->GetSize3D().Z / 2 : VisualizerComponent->GetSize2D().Y / 2);
	const float HalfLength = (Shape == EProjectionShape::Cuboid ? VisualizerComponent->GetSize3D().X / 2 : VisualizerComponent->GetSize2D().X / 2); // AKA radius for Cylinder shape
	const float HalfBreadth = VisualizerComponent->GetSize3D().Y / 2;
	constexpr int32 Sides = 32;
	constexpr uint8 DepthPriority = 0;
	constexpr float Thickness = 2;
	constexpr float DepthBias = 0;
	constexpr bool ScreenSpace = false;

	const FLinearColor RedColor = FLinearColor::Red;
	const float HollowHalfHeight = (Shape == EProjectionShape::Cuboid ? VisualizerComponent->GetHollowSize2D().X / 2 : VisualizerComponent->GetHollowSizeF() / 2); // AKA radius for Cylinder shape)
	const float HollowHalfBreadth = VisualizerComponent->GetHollowSize2D().Y / 2;

	const FRotationTranslationMatrix Matrix = FRotationTranslationMatrix(
		VisualizerComponent->GetOwner()->GetActorRotation(),
		ActorLocation); // Rotation and Translation Matrix
	
	switch (Shape)
	{
		case (EProjectionShape::Cylinder):
			{	
				DrawWireCylinder(PDI, ActorLocation, ActorRotationMatrix.GetScaledAxis(EAxis::X), ActorRotationMatrix.GetScaledAxis(EAxis::Y), ActorRotationMatrix.GetScaledAxis(EAxis::Z), Color, HalfLength, HalfHeight, Sides, DepthPriority, Thickness, DepthBias, ScreenSpace);
				break;
			}

		case (EProjectionShape::Cube):
			{
				const FBox CubeShape = FBox::BuildAABB(FVector::Zero(), FVector(HalfLength, HalfLength, HalfHeight));
				DrawWireBox(PDI, Matrix, CubeShape, Color, DepthPriority, Thickness, DepthBias, ScreenSpace);
				break;
			}

		case (EProjectionShape::Cuboid):
			{
				const FBox CuboidShape = FBox::BuildAABB(FVector::Zero(), FVector(HalfLength, HalfBreadth, HalfHeight));
				DrawWireBox(PDI, Matrix, CuboidShape, Color, DepthPriority, Thickness, DepthBias, ScreenSpace);
				break;
			}
	}

	// Visualizing the shape and hollow size
	if (VisualizerComponent->CanDrawHollow())
	{
		switch (Shape)
		{
		case (EProjectionShape::Cylinder):
			{	
				DrawWireCylinder(PDI, ActorLocation, ActorRotationMatrix.GetScaledAxis(EAxis::X), ActorRotationMatrix.GetScaledAxis(EAxis::Y), ActorRotationMatrix.GetScaledAxis(EAxis::Z), RedColor, HollowHalfHeight, HalfHeight, Sides, DepthPriority, Thickness, DepthBias, ScreenSpace);
				break;
			}

		case (EProjectionShape::Cube):
			{
				const FBox CubeShape = FBox::BuildAABB(FVector::Zero(), FVector(HollowHalfHeight, HollowHalfHeight, HalfHeight));
				DrawWireBox(PDI, Matrix, CubeShape, RedColor, DepthPriority, Thickness, DepthBias, ScreenSpace);
				break;
			}

		case (EProjectionShape::Cuboid):
			{
				const FBox CuboidShape = FBox::BuildAABB(FVector::Zero(), FVector(HollowHalfHeight, HollowHalfBreadth, HalfHeight));
				DrawWireBox(PDI, Matrix, CuboidShape, RedColor, DepthPriority, Thickness, DepthBias, ScreenSpace);
				break;
			}
		}
	}

	// Visualizing raycast lines
	if (VisualizerComponent->CanDrawRaycastLines())
	{
		TArray<FVector> Points = VisualizerComponent->GetPoints();
		
		for (FVector Point : Points)
		{
			FVector Start = ActorRotation.RotateVector(Point + FVector(0, 0, HalfHeight)) + ActorLocation;
			FMatrix LineMatrix = FScaleRotationTranslationMatrix(
				FVector::One(),
				ActorRotation,
				Start
			);

			DrawDirectionalArrowNegZ(PDI, LineMatrix, RedColor, MaxHeight, 15, DepthPriority, Thickness);
		}
	}
}
