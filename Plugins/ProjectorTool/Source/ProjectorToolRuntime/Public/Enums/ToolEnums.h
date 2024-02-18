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
enum class EInstanceAlignment : uint8
{
	Default,
	SurfaceNormal
};

UENUM()
enum class EInstanceScaleType : uint8
{
	MinToMax,
	MaxToMin
};