// All rights reserved.

#include "LookAtRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Twalien.h"

ULookAtRange::ULookAtRange()
{
	ForceInstancing(true);
}

void ULookAtRange::OnGameplayTaskActivated(UGameplayTask & Task)
{
}

void ULookAtRange::OnGameplayTaskDeactivated(UGameplayTask & Task)
{
}

void ULookAtRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent * Blackboard = OwnerComp.GetBlackboardComponent();
	TArray<FOverlapResult> OverlapResult;
	
	//Assign AIController
	if(!AIControllerReference)
	{
		AIControllerReference = OwnerComp.GetAIOwner();
	}

	if(DoTraceAction(AIControllerReference->GetPawn()->GetActorLocation(), AIControllerReference->GetPawn()->GetActorRotation().Quaternion(), OverlapResult))
	{
		SearchForPlayers(Blackboard, OverlapResult);
	}

}

bool ULookAtRange::DoTraceAction(FVector Position, FQuat Rotation, TArray<FOverlapResult> &OverlapResults)
{
	FCollisionObjectQueryParams QueryParams;
	FCollisionShape Shape;
	const UWorld * world = GetWorld();

	QueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);

	return world->OverlapMultiByObjectType(OverlapResults, Position, Rotation, QueryParams, Shape.MakeSphere(SightDistance));
}

void ULookAtRange::SearchForPlayers(UBlackboardComponent * Blackboard, const TArray<FOverlapResult> &OverlapResults)
{
	float ResultDistance;
	float MinDistance = SightDistance;
	ATwalien * Player;

	for (const FOverlapResult & Result : OverlapResults)
	{
		Player = Cast<ATwalien>(Result.Actor);
		if(Player)
		{
			ResultDistance = FVector::Dist(AIControllerReference->GetPawn()->GetActorLocation(), Player->GetActorLocation());
			if(ResultDistance < MinDistance)
			{
				DoTargettingAndRegisterFocus(Blackboard, Player);
				MinDistance = ResultDistance;
			}
		}
	}
}

void ULookAtRange::DoTargettingAndRegisterFocus(UBlackboardComponent * Blackboard, class ATwalien * Player)
{
	Blackboard->SetValueAsObject(TargetKey.SelectedKeyName, Player);

	if(Blackboard->GetValueAsObject(Player1Key.SelectedKeyName) == Player)
	{
		RegisterFocusInAlertSystem(Blackboard, Player1ReportedKey, Player1ReportAmountKey, Player2ReportedKey, Player2ReportAmountKey, Player2SightedKey);
	}
	else
	{
		RegisterFocusInAlertSystem(Blackboard, Player2ReportedKey, Player2ReportAmountKey, Player1ReportedKey, Player1ReportAmountKey, Player1SightedKey);
	}
}

void ULookAtRange::RegisterFocusInAlertSystem(UBlackboardComponent * Blackboard, FBlackboardKeySelector PlayerReportedKey, FBlackboardKeySelector PlayerReportAmountKey, FBlackboardKeySelector OtherPlayerReportedKey, FBlackboardKeySelector OtherPlayerReportAmountKey, FBlackboardKeySelector OtherPlayerSightedKey)
{
	const bool IsPlayerReported = Blackboard->GetValueAsBool(PlayerReportedKey.SelectedKeyName);
	const bool IsOtherPlayerReported = Blackboard->GetValueAsBool(OtherPlayerReportedKey.SelectedKeyName);

	if(!IsPlayerReported)
	{
		IncrementPlayerReference(Blackboard, PlayerReportedKey, PlayerReportAmountKey);

		if(IsOtherPlayerReported)
		{
			DecrementPlayerReference(Blackboard, OtherPlayerReportedKey, OtherPlayerReportAmountKey, OtherPlayerSightedKey);
		}
	}

}

void ULookAtRange::IncrementPlayerReference(UBlackboardComponent * Blackboard, FBlackboardKeySelector PlayerReportedKey, FBlackboardKeySelector PlayerReportAmountKey)
{
	const int NewReferencesAmount = Blackboard->GetValueAsInt(PlayerReportAmountKey.SelectedKeyName) + 1;

	Blackboard->SetValueAsBool(PlayerReportedKey.SelectedKeyName, true);
	Blackboard->SetValueAsInt(PlayerReportAmountKey.SelectedKeyName, NewReferencesAmount);
}

void ULookAtRange::DecrementPlayerReference(UBlackboardComponent * Blackboard, FBlackboardKeySelector PlayerReportedKey, FBlackboardKeySelector PlayerReportAmountKey, FBlackboardKeySelector PlayerSightedKey)
{
	const int NewReferencesAmount = Blackboard->GetValueAsInt(PlayerReportAmountKey.SelectedKeyName) - 1;

	Blackboard->SetValueAsBool(PlayerReportedKey.SelectedKeyName, false);
	Blackboard->SetValueAsInt(PlayerReportAmountKey.SelectedKeyName, NewReferencesAmount);

	if(NewReferencesAmount == 0)
	{
		Blackboard->SetValueAsBool(PlayerSightedKey.SelectedKeyName, false);
	}
}