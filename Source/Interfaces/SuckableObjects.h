// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SuckableObjects.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USuckableObjects : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TWALIENS_API ISuckableObjects
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	ISuckableObjects();

	virtual void OnBeingSucked(const class UDustpan & Dustpan) = 0;

	FORCEINLINE bool GetIsExclusive() const { return IsSuckExclusive; }
	FORCEINLINE void SetIsExclusive(bool NewValueIsExclusive) { IsSuckExclusive = NewValueIsExclusive; }
	FORCEINLINE bool GetIsSucked() const { return IsSucked; }
	FORCEINLINE bool GetIsSkillAffected() const { return IsSkillAffected; }

protected:
	bool IsSuckExclusive;
	bool IsSkillAffected;
	bool IsSucked;
};
