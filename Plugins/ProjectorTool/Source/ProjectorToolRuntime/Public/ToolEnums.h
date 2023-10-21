#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EProjectionShape : uint8
{
	Cylinder,
	Cube,
	Cuboid
};

UENUM()
enum class EMeshAlignment : uint8
{
	Default,
	SurfaceNormal,
	ObjectReference
};