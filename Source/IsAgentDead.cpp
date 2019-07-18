// All rights reserved.

#include "IsAgentDead.h"
#include "AIController.h"
#include "LifeComponent.h"

UIsAgentDead::UIsAgentDead()
{
	ForceInstancing(true);
}

void UIsAgentDead::OnGameplayTaskActivated(UGameplayTask & Task)
{
	
}

void UIsAgentDead::OnGameplayTaskDeactivated(UGameplayTask & Task)
{
	
}

bool UIsAgentDead::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	AActor * Actor = OwnerComp.GetAIOwner()->GetPawn();
	bool result = true;
	TSet<UActorComponent*> Components;
	ILifeFunctionality * Life = nullptr;

	Components = Actor->GetComponents();
	for (UActorComponent * Component : Components)
	{
		Life = Cast<ILifeFunctionality>(Component);
		if (Life)
		{
			break;
		}
	}

	if(Life)
	{
		result = Life->GetHitPoints() <= 0;
	}

	return result;
}