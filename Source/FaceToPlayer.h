// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "FaceToPlayer.generated.h"

/**
 * 
 */
UCLASS()
class TWALIENS_API UFaceToPlayer : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UFaceToPlayer();

protected:
	virtual void OnGameplayTaskActivated(class UGameplayTask & Task) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Task Setting")
	FBlackboardKeySelector TargetKey;
};
