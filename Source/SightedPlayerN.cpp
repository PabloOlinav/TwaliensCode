// All rights reserved.

#include "SightedPlayerN.h"
#include "BehaviorTree/BlackboardComponent.h"

USightedPlayerN::USightedPlayerN()
{
	ForceInstancing(true);
}

void USightedPlayerN::OnGameplayTaskActivated(UGameplayTask & Task)
{
}

void USightedPlayerN::OnGameplayTaskDeactivated(UGameplayTask & Task)
{
}

bool USightedPlayerN::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	const UBlackboardComponent * Blackboard = OwnerComp.GetBlackboardComponent();

	return Blackboard->GetValueAsInt(ChoosenSightKey.SelectedKeyName) == PlayerIndex;
}