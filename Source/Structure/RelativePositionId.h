// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "RelativePositionId.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FRelativePositionId
{
	GENERATED_USTRUCT_BODY()

	FRelativePositionId()
	{
		QuadrantIndex = RingIndex = PositionIndex = 0;
	}

	FRelativePositionId(int Quadrant, int Ring, int Position)
	{
		QuadrantIndex = Quadrant;
		RingIndex = Ring;
		PositionIndex = Position;
	}

	UPROPERTY(BlueprintReadWrite)
	int QuadrantIndex;

	UPROPERTY(BlueprintReadWrite)
	int RingIndex;

	UPROPERTY(BlueprintReadWrite)
	int PositionIndex;
};