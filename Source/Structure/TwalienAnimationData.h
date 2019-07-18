// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "TwalienAnimationData.generated.h"

/**
 * Struct that encapsulates animation info for Twalien Class.
 */
USTRUCT(BlueprintType)
struct FTwalienAnimationData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	float ForwardVector;

	UPROPERTY(BlueprintReadOnly)
	float RightVector;

	UPROPERTY(BlueprintReadOnly)
	bool HasTakenDamage;

	UPROPERTY(BlueprintReadOnly)
	bool IsDeath;

	UPROPERTY(BlueprintReadOnly)
	bool IsUsingTheirWeapon;

	UPROPERTY(BlueprintReadOnly)
	bool IsHoldingShoot;

	UPROPERTY(BlueprintReadOnly)
	bool BoredForWaitingInput;

	UPROPERTY(BlueprintReadOnly)
	bool IsDustpanPlayer;

	UPROPERTY(BlueprintReadOnly)
	bool IsSwitchingWeapons;
};
