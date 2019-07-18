// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BasicWarriorAnimationData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FBasicWarriorAnimationData
{
	GENERATED_USTRUCT_BODY()

	FBasicWarriorAnimationData();

	FBasicWarriorAnimationData(bool NewHasTakenDamage, bool NewIsDeath, bool NewIsUsingTheirWeapon, bool NewIsDisconnected);

	UPROPERTY(BlueprintReadOnly)
	bool HasTakenDamage;

	UPROPERTY(BlueprintReadOnly)
	bool IsDead;

	UPROPERTY(BlueprintReadOnly)
	bool IsUsingTheirWeapon;

	UPROPERTY(BlueprintReadOnly)
	bool IsDisconnected;
};
