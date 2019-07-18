// All rights reserved.

#include "IsAttacking.h"
#include "BasicWarrior.h"
#include "AIController.h"
#include "CommonAIControllerActions.h"

void UIsAttacking::OnGameplayTaskActivated(UGameplayTask & Task)
{

}

void UIsAttacking::OnGameplayTaskDeactivated(UGameplayTask & Task)
{

}

UIsAttacking::UIsAttacking()
{
	ForceInstancing(true);
}

bool UIsAttacking::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	if (!Controller)
	{
		Controller = Cast<ICommonAIControllerActions>(OwnerComp.GetAIOwner());
	}

	return Controller && Controller->GetAgentIsAttacking();
}