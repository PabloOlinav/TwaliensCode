// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PoolManager.h"
#include "IPooledObject.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIPooledObject : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TWALIENS_API IIPooledObject
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	FORCEINLINE void SetOwnerPool(APoolManager & Pool) { OwnerPool = &Pool; }
	FORCEINLINE void ReturnToPool() { if (OwnerPool) OwnerPool->ReturnActorToPool(this); }
	FORCEINLINE bool HasValidPool() { if (IsValid(OwnerPool)) return true; return false; };

	virtual void OnObjectExitsPool() = 0;
	virtual void OnObjectEnterPool() = 0;
protected:
	APoolManager * OwnerPool;
};
