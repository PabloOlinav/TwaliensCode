// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ClearTargetField.generated.h"

/**
 *
 */
UCLASS()
class TWALIENS_API UClearTargetField : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UClearTargetField();

protected:
	virtual void OnGameplayTaskActivated(class UGameplayTask & Task) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Task Property")
	FBlackboardKeySelector TargetKey;
};
