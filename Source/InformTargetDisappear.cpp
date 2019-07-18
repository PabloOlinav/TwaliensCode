// All rights reserved.

#include "InformTargetDisappear.h"
#include "BehaviorTree/BlackboardComponent.h"

UInformTargetDisappear::UInformTargetDisappear()
{
	ForceInstancing(true);
}

void UInformTargetDisappear::OnGameplayTaskActivated(class UGameplayTask & Task)
{
}

EBTNodeResult::Type UInformTargetDisappear::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	UBlackboardComponent * Blackboard = OwnerComp.GetBlackboardComponent();
	const bool Player1Reported = Blackboard->GetValueAsBool(Player1ReportedKey.SelectedKeyName);
	const bool Player2Reported = Blackboard->GetValueAsBool(Player2ReportedKey.SelectedKeyName);
	const EBTNodeResult::Type Exit = EBTNodeResult::Succeeded;

	if(Player1Reported)
	{
		DecrementPlayerReference(Blackboard, Player1ReportedKey, Player1ReportAmountKey, Player1SightedKey);
	}
	else if(Player2Reported)
	{
		DecrementPlayerReference(Blackboard, Player2ReportedKey, Player2ReportAmountKey, Player2SightedKey);
	}

	return Exit;
}

void UInformTargetDisappear::DecrementPlayerReference(UBlackboardComponent * Blackboard, const FBlackboardKeySelector & PlayerReportedKey, const FBlackboardKeySelector & PlayerReportAmountKey, const FBlackboardKeySelector & PlayerSightedKey)
{
	const int NewReferenceValue = Blackboard->GetValueAsInt(PlayerReportAmountKey.SelectedKeyName) - 1;

	Blackboard->SetValueAsBool(PlayerReportedKey.SelectedKeyName, false);
	Blackboard->SetValueAsInt(PlayerReportAmountKey.SelectedKeyName, NewReferenceValue);
	if(NewReferenceValue == 0)
	{
		Blackboard->SetValueAsBool(PlayerSightedKey.SelectedKeyName, false);
	}
}