// All rights reserved.

#include "SelectSight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

USelectSight::USelectSight()
{
	ForceInstancing(true);
}

void USelectSight::OnGameplayTaskActivated(UGameplayTask & Task)
{
	
}
void USelectSight::OnGameplayTaskDeactivated(UGameplayTask & Task)
{
	
}

void USelectSight::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent * Blackboard = OwnerComp.GetBlackboardComponent();
	const bool IsPlayer1Sighted = Blackboard->GetValueAsBool(Player1SightedKey.SelectedKeyName);
	const bool IsPlayer2Sighted = Blackboard->GetValueAsBool(Player2SightedKey.SelectedKeyName);
	const int ChoosenSight = Blackboard->GetValueAsInt(ChoosenSightKey.SelectedKeyName);
	const FVector Player1Position = Blackboard->GetValueAsVector(Player1LastPositionKey.SelectedKeyName);
	const FVector Player2Position = Blackboard->GetValueAsVector(Player2LastPositionKey.SelectedKeyName);
	const APawn * ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();

	//Caso de que ambos esten avistados pero el npc todavia no eligio a ninguno.
	if(IsPlayer1Sighted && IsPlayer2Sighted && ChoosenSight == -1)
	{
		const float Distance1 = FVector::Dist(ControlledPawn->GetActorLocation(), Player1Position);
		const float Distance2 = FVector::Dist(ControlledPawn->GetActorLocation(), Player2Position);

		if(Distance1 <= Distance2)
		{
			Blackboard->SetValueAsInt(ChoosenSightKey.SelectedKeyName, 0);
		}
		else
		{
			Blackboard->SetValueAsInt(ChoosenSightKey.SelectedKeyName, 1);
		}
	}
	//Caso de que solo se avisto al player 1 y el npc todavia no ha elegido a ninguno.
	else if(IsPlayer1Sighted && ChoosenSight == -1)
	{
		Blackboard->SetValueAsInt(ChoosenSightKey.SelectedKeyName, 0);
	}
	//Caso de que solo se avisto al player 2 y el npc todavia no ha elegido a ninguno.
	else if (IsPlayer2Sighted && ChoosenSight == -1)
	{
		Blackboard->SetValueAsInt(ChoosenSightKey.SelectedKeyName, 1);
	}
	//Caso de que no haya avistamientos.
	else if(!IsPlayer1Sighted && !IsPlayer2Sighted)
	{
		Blackboard->SetValueAsInt(ChoosenSightKey.SelectedKeyName, -1);
	}
	//Caso de que el player 1 fuera avistado, dejara de estarlo y ahora el player 2 esta siendo avistado.
	else if(ChoosenSight == 0 && !IsPlayer1Sighted && IsPlayer2Sighted)
	{
		Blackboard->SetValueAsInt(ChoosenSightKey.SelectedKeyName, 1);
	}
	//Caso de que el player 2 fuera avistado, dejara de estarlo y ahora el player 1 esta siendo avistado.
	else if(ChoosenSight == 1 && !IsPlayer2Sighted && IsPlayer1Sighted)
	{
		Blackboard->SetValueAsInt(ChoosenSightKey.SelectedKeyName, 0);
	}
	//Caso en el que estan avistados ambos y el npc eligio el jugador 1.
	else if(IsPlayer1Sighted && IsPlayer2Sighted && ChoosenSight == 0)
	{
		FocusChange(Blackboard, ControlledPawn->GetActorLocation(), Player1Position, Player2Position, 1);
	}
	//Caso en el que estan avistados ambos y el npc eligio el jugador 2.
	else if (IsPlayer1Sighted && IsPlayer2Sighted && ChoosenSight == 1)
	{
		FocusChange(Blackboard, ControlledPawn->GetActorLocation(), Player2Position, Player1Position, 0);
	}
}

void USelectSight::FocusChange(UBlackboardComponent * Blackboard, FVector NPCPosition, FVector CurrentFocusPosition, FVector OtherPosition, int NewValue) const
{
	const float CurrentFocusDistance = FVector::Dist(NPCPosition, CurrentFocusPosition) * DeltaSpace;
	const float OtherFocusDistance = FVector::Dist(NPCPosition, OtherPosition);

	if(CurrentFocusDistance >= OtherFocusDistance)
	{
		Blackboard->SetValueAsInt(ChoosenSightKey.SelectedKeyName, NewValue);
	}
}