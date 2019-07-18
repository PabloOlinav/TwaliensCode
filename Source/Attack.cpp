// All rights reserved.

#include "Attack.h"
#include "BasicWarrior.h"
#include "AIController.h"
#include "CommonAIControllerActions.h"

void UAttack::OnGameplayTaskActivated(UGameplayTask & Task)
{
}

UAttack::UAttack()
{
	ForceInstancing(true);
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

void UAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!Controller->GetAgentIsAttacking())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}


void UAttack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
	
	if (TaskResult != EBTNodeResult::Succeeded)
	{
		Controller->SetAgentIsAttacking(false);
	}
}

EBTNodeResult::Type UAttack::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type result;

	if (!Controller)
	{
		Controller = Cast<ICommonAIControllerActions>(OwnerComp.GetAIOwner());
	}


	if(Controller)
	{
		if(!Controller->GetAgentIsAttacking())
		{
			Controller->SetAgentIsAttacking(true);
		}
		result = EBTNodeResult::Type::InProgress;
	}
	else
	{
		result = EBTNodeResult::Type::Failed;
	}

	return result;
}