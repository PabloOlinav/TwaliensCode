// All rights reserved.

#include "ReportPlayerPosition.h"
#include "BehaviorTree/BlackboardComponent.h"

UReportPlayerPosition::UReportPlayerPosition()
{
	ForceInstancing(true);
}

void UReportPlayerPosition::OnGameplayTaskActivated(UGameplayTask & Task)
{
}
void UReportPlayerPosition::OnGameplayTaskDeactivated(UGameplayTask & Task)
{
}

void UReportPlayerPosition::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent * Blackboard = OwnerComp.GetBlackboardComponent();
	const bool Player1Reported = Blackboard->GetValueAsBool(Player1ReportedKey.SelectedKeyName);
	const bool Player2Reported = Blackboard->GetValueAsBool(Player2ReportedKey.SelectedKeyName);
	
	if(Player1Reported)
	{
		InformAboutPlayer(Blackboard, Player1SightedKey, Player1Key, Player1LastPositionKey);
	}
	else if(Player2Reported)
	{
		InformAboutPlayer(Blackboard, Player2SightedKey, Player2Key, Player2LastPositionKey);
	}
}

void UReportPlayerPosition::InformAboutPlayer(UBlackboardComponent * Blackboard, FBlackboardKeySelector PlayerSightedKey, FBlackboardKeySelector PlayerKey, FBlackboardKeySelector PlayerLastPositionKey) const
{
	const AActor * Player = Cast<AActor>(Blackboard->GetValueAsObject(PlayerKey.SelectedKeyName));
	Blackboard->SetValueAsBool(PlayerSightedKey.SelectedKeyName, true);
	Blackboard->SetValueAsVector(PlayerLastPositionKey.SelectedKeyName, Player->GetActorLocation());
}