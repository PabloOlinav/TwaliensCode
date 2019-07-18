// All rights reserved.

#include "WaitPermanently.h"

UWaitPermanently::UWaitPermanently()
{
	ForceInstancing(true);
	bNotifyTick = true;
}

void UWaitPermanently::OnGameplayTaskActivated(UGameplayTask& Task)
{}

void UWaitPermanently::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

EBTNodeResult::Type UWaitPermanently::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	return EBTNodeResult::InProgress;
}
