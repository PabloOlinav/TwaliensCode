// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LifeFunctionality.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActorLifeHasBeenUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActorHasDied);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULifeFunctionality : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TWALIENS_API ILifeFunctionality
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//Return Actor Hit Points.
	virtual float GetHitPoints() const = 0;

	//Reset Actor Hit Points.
	virtual void ResetHitPoints() = 0;

	virtual float GetMaxHitPoints() const = 0;

	//Modify Actor Hit Points. 
	//DeltaHitPoints -> Hit Points added to Actor Hit Points.
	virtual void SetHitPoints(float DeltaHitPoints) = 0;

	//Add a function to life changes delegate.
	virtual void AddDelegateToHPUpdate(const FScriptDelegate & Delegate) = 0;

	//Add a function to delegate that broadcast whenever actor dies.
	virtual void AddDelegateToDeath(const FScriptDelegate & Delegate) = 0;

	//Remove a function from life changes delegate.
	virtual void RemoveDelegateFromHPUpdate(const FScriptDelegate & Delegate) = 0;

	//Clear all functions from life changes delegate.
	virtual void RemoveAllDelegatesFromHPUpdate() = 0;

	//Remove a function from death delegate.
	virtual void RemoveDelegateFromDeath(const FScriptDelegate & Delegate) = 0;

	//Clear all functions from death delegate.
	virtual void RemoveAllDelegatesFromDeath() = 0;
};
