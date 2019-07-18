// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "WaitPermanently.generated.h"

/**
 * 
 */
UCLASS()
class TWALIENS_API UWaitPermanently : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UWaitPermanently();

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
