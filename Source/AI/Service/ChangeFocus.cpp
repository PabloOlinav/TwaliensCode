// All rights reserved.

#include "ChangeFocus.h"
#include "Twalien.h"
#include "BrainComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Structure/TwalienAnimationData.h"
#include "GameFramework/Controller.h"
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "Components/RadialAttackableLocations.h"

UChangeFocus::UChangeFocus() : MustChangeFocusOnSwitch(true), MustChangeFocusWithDistance(true), DistanceDelta(0.3f)
{
	ForceInstancing(true);
}

void UChangeFocus::OnGameplayTaskActivated(UGameplayTask & Task)
{
}

void UChangeFocus::OnGameplayTaskDeactivated(UGameplayTask & Task)
{
}

void UChangeFocus::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//Get target, animation data from target and if we are focusing on weapon player
	const ATwalien * Target = Cast<ATwalien>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));
	if (Target)
	{
		FTwalienAnimationData AnimationData = Target->GetAnimationData();
		const bool WeaponFocus = OwnerComp.GetBlackboardComponent()->GetValueAsBool(WeaponFocusKey.SelectedKeyName);
		UBlackboardComponent * Blackboard = OwnerComp.GetBlackboardComponent();
		ATwalien * Player1 = Cast<ATwalien>(Blackboard->GetValueAsObject(Player1ReferenceKey.SelectedKeyName));
		ATwalien * Player2 = Cast<ATwalien>(Blackboard->GetValueAsObject(Player2ReferenceKey.SelectedKeyName));

		//Y we must change our focus on switch weapons and players change their weapons
		if (MustChangeFocusOnSwitch && ((AnimationData.IsDustpanPlayer && WeaponFocus) || (!AnimationData.IsDustpanPlayer && !WeaponFocus)))
		{
			ChangeFocusOnSwitch(*Blackboard, *Player1, *Player2, *Target);
		}

		if (MustChangeFocusWithDistance)
		{
			ChangeFocusOnDistance(OwnerComp, *Blackboard, *Player1, *Player2, *Target);
		}
	}
}

void UChangeFocus::ChangeFocusOnSwitch(UBlackboardComponent & Blackboard, ATwalien & Player1, ATwalien & Player2, const ATwalien & Target)
{
	//If we had player1 as target 
	if (&Target == &Player1)
	{
		//Do free stuff
		FreeStrategicPointFromTarget(Blackboard, Player1);

		//change to player2
		Blackboard.SetValueAsObject(TargetKey.SelectedKeyName, &Player2);
	}
	else
	{
		//Do free stuff
		FreeStrategicPointFromTarget(Blackboard, Player2);

		//change to player1
		Blackboard.SetValueAsObject(TargetKey.SelectedKeyName, &Player1);
	}
}

void UChangeFocus::ChangeFocusOnDistance(UBehaviorTreeComponent& OwnerComp, UBlackboardComponent & Blackboard, ATwalien & Player1, ATwalien & Player2, const ATwalien & Target)
{
	//Get a const pointer to both player
	APawn * Agent = OwnerComp.GetAIOwner()->GetPawn();

	//Calculate distances
	const float DistancePlayer1 = FVector::Distance(Agent->GetActorLocation(), Player1.GetActorLocation());
	const float DistancePlayer2 = FVector::Distance(Agent->GetActorLocation(), Player2.GetActorLocation());

	//If our target is player1
	if (&Target == &Player1)
	{
		//Calculate threshold
		const float ThresholdDistance = DistancePlayer1 * DistanceDelta;

		//If player 2 distance beat that threshold
		if (DistancePlayer2 < ThresholdDistance)
		{
			//Do free stuff
			FreeStrategicPointFromTarget(Blackboard, Player1);

			ChangeNewFocus(Target, Blackboard, Player2);
		}
	}
	else if(&Target == &Player2)
	{
		//Calculate threshold
		const float ThresholdDistance = DistancePlayer2 * DistanceDelta;

		//If player 1 distance beat that threshold
		if (DistancePlayer1 < ThresholdDistance)
		{
			//do free stuff
			FreeStrategicPointFromTarget(Blackboard, Player2);

			ChangeNewFocus(Target, Blackboard, Player1);
		}
	}
}

void UChangeFocus::ChangeNewFocus(const class ATwalien & Target, class UBlackboardComponent & Blackboard, class ATwalien & NewTarget)
{
	//Set player 2 as new target
	Blackboard.SetValueAsObject(TargetKey.SelectedKeyName, &NewTarget);

	//get if target has dustpan
	const bool DustpanPlayer = NewTarget.GetAnimationData().IsDustpanPlayer;

	//if target has dustpan increment dustpan references and decrement weapon references
	if (DustpanPlayer)
	{
		RegisterFocus(DustpanFollowersKey, WeaponFollowersKey, Blackboard, false);
	}
	//if target has dustpan increment weapon references and decrement dustpan references
	else
	{
		RegisterFocus(WeaponFollowersKey, DustpanFollowersKey, Blackboard, true);
	}
}

void UChangeFocus::RegisterFocus(const FBlackboardKeySelector & IncrementFollowers, const FBlackboardKeySelector & DecrementFollowers, class UBlackboardComponent & Blackboard, bool NewWeaponFocusValue)
{
	//Calculate new references count
	const int NewIncrementedAmopunt = Blackboard.GetValueAsInt(IncrementFollowers.SelectedKeyName) + 1;
	const int NewDecrementedAmount = Blackboard.GetValueAsInt(DecrementFollowers.SelectedKeyName) - 1;

	//set new references to blackboard
	Blackboard.SetValueAsInt(IncrementFollowers.SelectedKeyName, NewIncrementedAmopunt);
	Blackboard.SetValueAsInt(DecrementFollowers.SelectedKeyName, NewDecrementedAmount);
	
	//Set new value to focus weapon entry
	Blackboard.SetValueAsBool(WeaponFocusKey.SelectedKeyName, NewWeaponFocusValue);
}

void UChangeFocus::FreeStrategicPointFromTarget(class UBlackboardComponent & Blackboard, class AActor & Target)
{
	//if we chased player
	if (Blackboard.GetValueAsBool(TargetNearPositionSetKey.SelectedKeyName))
	{
		//free relative position on target and flag
		UActorComponent * Component = Target.GetComponentByClass(URadialAttackableLocations::StaticClass());
		if (Component)
		{
			URadialAttackableLocations * RadialPointsComponent = Cast<URadialAttackableLocations>(Component);
			if (RadialPointsComponent)
			{
				//Free position
				RadialPointsComponent->FreeRelativePosition(
					FRelativePositionId(Blackboard.GetValueAsInt(TargetRadialQuadrantKey.SelectedKeyName), Blackboard.GetValueAsInt(TargetRadialRingKey.SelectedKeyName), Blackboard.GetValueAsInt(TargetRadialPositionKey.SelectedKeyName))
				);

				//put correct value in flag
				Blackboard.SetValueAsBool(TargetNearPositionSetKey.SelectedKeyName, false);
			}
		}
	}
}
