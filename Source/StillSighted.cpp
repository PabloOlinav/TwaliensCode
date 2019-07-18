// All rights reserved.

#include "StillSighted.h"
#include "BehaviorTree/BlackboardComponent.h"

UStillSighted::UStillSighted()
{
	ForceInstancing(true);
}

void UStillSighted::OnGameplayTaskActivated(UGameplayTask & Task)
{
}

void UStillSighted::OnGameplayTaskDeactivated(UGameplayTask & Task)
{
}

bool UStillSighted::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	const UBlackboardComponent * Blackboard = OwnerComp.GetBlackboardComponent();

	return Blackboard->GetValueAsBool(PlayerSightedKey.SelectedKeyName);
}