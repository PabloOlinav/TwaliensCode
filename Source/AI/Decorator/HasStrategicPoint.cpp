// All rights reserved.

#include "HasStrategicPoint.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UHasStrategicPoint::UHasStrategicPoint()
{
	ForceInstancing(true);
}

void UHasStrategicPoint::OnGameplayTaskActivated(UGameplayTask& Task)
{

}

void UHasStrategicPoint::OnGameplayTaskDeactivated(UGameplayTask& Task)
{

}

bool UHasStrategicPoint::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	//Get blackboard
	UBlackboardComponent * Blackboard = OwnerComp.GetBlackboardComponent();

	return Blackboard->GetValueAsBool(TargetNearPositionSetKey.SelectedKeyName);
}
