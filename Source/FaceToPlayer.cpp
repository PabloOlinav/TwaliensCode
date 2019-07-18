// All rights reserved.

#include "FaceToPlayer.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"

UFaceToPlayer::UFaceToPlayer()
{
	ForceInstancing(true);
}

void UFaceToPlayer::OnGameplayTaskActivated(class UGameplayTask & Task)
{	
}
	
EBTNodeResult::Type UFaceToPlayer::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	APawn * Agent = OwnerComp.GetAIOwner()->GetPawn();
	const UBlackboardComponent * Blackboard = OwnerComp.GetBlackboardComponent();
	const AActor * Target = Cast<AActor>(Blackboard->GetValueAsObject(TargetKey.SelectedKeyName));
	const FRotator NewRotator = UKismetMathLibrary::FindLookAtRotation(Agent->GetActorLocation(), Target->GetActorLocation());
	
	Agent->SetActorRotation(NewRotator);
	return EBTNodeResult::Succeeded;
}