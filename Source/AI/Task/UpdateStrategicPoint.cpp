// All rights reserved.

#include "UpdateStrategicPoint.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"
#include "BrainComponent.h"
#include "GameFramework/Controller.h"
#include "Twalien.h"
#include "Components/RadialAttackableLocations.h"
#include "AIController.h"


UUpdateStrategicPoint::UUpdateStrategicPoint()
{
	ForceInstancing(true);
}

void UUpdateStrategicPoint::OnGameplayTaskActivated(UGameplayTask& Task)
{
}

EBTNodeResult::Type UUpdateStrategicPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
			//if strategic info is set 
			if (Blackboard->GetValueAsBool(TargetNearPositionSetKey.SelectedKeyName))
			{
				//Get component for update strategic position
				URadialAttackableLocations * StrategicPointComponent = GetTargetComponent(*Player);

				//if player really has this component
				if (StrategicPointComponent)
				{
					//build strategic point access data
					FRelativePositionId AccessInfo(Blackboard->GetValueAsInt(TargetRadialQuadrantKey.SelectedKeyName), Blackboard->GetValueAsInt(TargetRadialRingKey.SelectedKeyName), Blackboard->GetValueAsInt(TargetRadialPositionKey.SelectedKeyName));
					FVector StrategicPosition;

					//update strategic point
					StrategicPointComponent->GetRelativePosition(AccessInfo, StrategicPosition);

					//set strategic position
					Blackboard->SetValueAsVector(TargetNearPositionKey.SelectedKeyName, Player->GetActorLocation() + StrategicPosition);


					//if we reach this line all went ok
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}

	return EBTNodeResult::Failed;
}

URadialAttackableLocations * UUpdateStrategicPoint::GetTargetComponent(const AActor & Target) const
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
