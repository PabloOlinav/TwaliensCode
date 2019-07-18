// All rights reserved.

#include "ClearTargetField.h"
#include "BehaviorTree/BlackboardComponent.h"

UClearTargetField::UClearTargetField()
{
	ForceInstancing(true);
}

void UClearTargetField::OnGameplayTaskActivated(class UGameplayTask & Task)
{
	
}

EBTNodeResult::Type UClearTargetField::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TargetKey.SelectedKeyName, nullptr);
	return EBTNodeResult::Succeeded;
}
