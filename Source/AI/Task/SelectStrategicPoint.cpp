// All rights reserved.

#include "SelectStrategicPoint.h"
#include "Twalien.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/RadialAttackableLocations.h"
#include "GameFramework/Actor.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BrainComponent.h"
#include "GameFramework/Controller.h"
#include "AIController.h"

USelectStrategicPoint::USelectStrategicPoint()
{
	ForceInstancing(true);
}

void USelectStrategicPoint::OnGameplayTaskActivated(UGameplayTask& Task)
{
}

EBTNodeResult::Type USelectStrategicPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	//Get blackboard
	UBlackboardComponent * Blackboard = OwnerComp.GetBlackboardComponent();
	//Get Controlled Agent
	APawn * ControlledAgent = OwnerComp.GetAIOwner()->GetPawn();
	//Get focused player and its component
	const ATwalien * Player = Cast<ATwalien>(Blackboard->GetValueAsObject(TargetKey.SelectedKeyName));

	//if blackboard exists
	if (Blackboard)
	{
		//if player exists
		if (Player)
		{
			URadialAttackableLocations * StrategicPointComponent = GetTargetComponent(*Player);

			//if player really has this component
			if (StrategicPointComponent)
			{
				//Query a new strategic point and its metada
				FVector StrategicPoint;
				FRelativePositionId AccessInfo;
				bool QueryReturnValue = StrategicPointComponent->GetFreeRelativePosition(ControlledAgent->GetActorLocation(), StrategicPoint, AccessInfo);

				//if query is successful
				if (QueryReturnValue)
				{
					//set strategic position
					Blackboard->SetValueAsVector(TargetNearPositionKey.SelectedKeyName, Player->GetActorLocation() + StrategicPoint);
					//set if strategic position is set
					Blackboard->SetValueAsBool(TargetNearPositionSetKey.SelectedKeyName, QueryReturnValue);
					//set strategic position access metada
					Blackboard->SetValueAsInt(TargetRadialQuadrantKey.SelectedKeyName, AccessInfo.QuadrantIndex);
					Blackboard->SetValueAsInt(TargetRadialRingKey.SelectedKeyName, AccessInfo.RingIndex);
					Blackboard->SetValueAsInt(TargetRadialPositionKey.SelectedKeyName, AccessInfo.PositionIndex);

					//if we reach this line all went ok
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}

	return EBTNodeResult::Failed;
}

class URadialAttackableLocations * USelectStrategicPoint::GetTargetComponent(const class AActor & Target) const
{
	URadialAttackableLocations * StrategicPointsComponent = nullptr;
	//Get component
	UActorComponent * Component = Target.GetComponentByClass(URadialAttackableLocations::StaticClass());
	if (Component)
	{
		StrategicPointsComponent = Cast<URadialAttackableLocations>(Component);
	}

	return StrategicPointsComponent;
}
