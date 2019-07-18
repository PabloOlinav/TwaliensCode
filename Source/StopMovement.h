// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "StopMovement.generated.h"

/**
 * 
 */
UCLASS()
class TWALIENS_API UStopMovement : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UStopMovement();

protected:
	virtual void OnGameplayTaskActivated(class UGameplayTask & Task) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) override;
};
