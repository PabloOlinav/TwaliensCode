// All rights reserved.

#include "StopMovement.h"
#include "AIController.h"

UStopMovement::UStopMovement()
{
	ForceInstancing(true);
}

void UStopMovement::OnGameplayTaskActivated(class UGameplayTask & Task)
{
}

EBTNodeResult::Type UStopMovement::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	OwnerComp.GetAIOwner()->StopMovement();
	return EBTNodeResult::Type::Succeeded;
}