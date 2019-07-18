// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CommonAIControllerActions.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCommonAIControllerActions : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TWALIENS_API ICommonAIControllerActions
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool GetAgentIsAttacking() = 0;
	virtual void SetAgentIsAttacking(bool NewAttackStatus) = 0;
	virtual void SetAgentTarget(class AActor * Target) = 0;
	virtual void RemoveAgentTarget(class AActor * Target) = 0;
	virtual void AlertPlayersInSight(bool InSight) = 0;
};
