// All rights reserved.

#include "SetFocus.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Twalien.h"
#include "Components/RadialAttackableLocations.h"

USetFocus::USetFocus() : FocusRatio(.7f)
{
	ForceInstancing(true);
}

void USetFocus::OnGameplayTaskActivated(UGameplayTask & Task)
{
}

void USetFocus::OnGameplayTaskDeactivated(UGameplayTask & Task)
{
}

void USetFocus::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//Get blackboard component
	UBlackboardComponent * Blackboard = OwnerComp.GetBlackboardComponent();
	const bool PlayerSighted = Blackboard->GetValueAsBool(PlayerSightedKey.SelectedKeyName);
	const UObject * Target = Blackboard->GetValueAsObject(TargetKey.SelectedKeyName);

	//initialize player pointer
	if (!Player1)
	{
		Player1 = Cast<ATwalien>(Blackboard->GetValueAsObject(Player1ReferenceKey.SelectedKeyName));
		Player2 = Cast<ATwalien>(Blackboard->GetValueAsObject(Player2ReferenceKey.SelectedKeyName));
	}

	//check if we dont have player sighted
	if (!PlayerSighted && Target)
	{
		CancelTarget(*Blackboard);
	}
	else if(PlayerSighted && !Target)
	{
		ChooseTarget(*Blackboard);
	}

}

void USetFocus::CancelTarget(UBlackboardComponent & Blackboard)
{
	ATwalien * FocusedPlayer = Cast<ATwalien>(Blackboard.GetValueAsObject(TargetKey.SelectedKeyName));

	//Set target field to null
	Blackboard.SetValueAsObject(TargetKey.SelectedKeyName, nullptr);

	//if we chased player
	if (Blackboard.GetValueAsBool(TargetNearPositionSetKey.SelectedKeyName))
	{
		//free relative position and flag
		UActorComponent * Component = FocusedPlayer->GetComponentByClass(URadialAttackableLocations::StaticClass());
		if (Component)
		{
			URadialAttackableLocations * StrategicLocationComponent = Cast<URadialAttackableLocations>(Component);
			if (StrategicLocationComponent)
			{
				//Free position
				StrategicLocationComponent->FreeRelativePosition(
					FRelativePositionId(Blackboard.GetValueAsInt(TargetRadialQuadrantKey.SelectedKeyName), Blackboard.GetValueAsInt(TargetRadialRingKey.SelectedKeyName), Blackboard.GetValueAsInt(TargetRadialPositionKey.SelectedKeyName))
				);

				//set get strategic position flag to false
				Blackboard.SetValueAsBool(TargetNearPositionSetKey.SelectedKeyName, false);
			}
		}
	}

	//If our target has weapon 
	if (Blackboard.GetValueAsBool(WeaponFocusKey.SelectedKeyName))
	{
		//Increment by 1 weapon references amount
		const int NewReferencesValue = Blackboard.GetValueAsInt(WeaponFollowersKey.SelectedKeyName) - 1;
		Blackboard.SetValueAsInt(WeaponFollowersKey.SelectedKeyName, NewReferencesValue);
	}
	else
	{
		//increment by 1 dustpan references amount
		const int NewReferencesValue = Blackboard.GetValueAsInt(DustpanFollowersKey.SelectedKeyName) - 1;
		Blackboard.SetValueAsInt(DustpanFollowersKey.SelectedKeyName, NewReferencesValue);
	}
}

void USetFocus::ChooseTarget(UBlackboardComponent & Blackboard)
{
	//Get Weapon and dustpan references amount
	const int DustpanFollowers = Blackboard.GetValueAsInt(DustpanFollowersKey.SelectedKeyName);
	const int WeaponFollowers = Blackboard.GetValueAsInt(WeaponFollowersKey.SelectedKeyName);

	if (WeaponFollowers < (WeaponFollowers + DustpanFollowers) * FocusRatio)
	{
		RegisterFocus(Blackboard, WeaponFollowersKey, true);
	}
	else
	{
		RegisterFocus(Blackboard, DustpanFollowersKey, false);
	}
}

void USetFocus::RegisterFocus(UBlackboardComponent & Blackboard, FBlackboardKeySelector IncrementFollowersKey, bool NewWeaponFocusValue)
{
	//Increment references
	const int NewFollowersAmount = Blackboard.GetValueAsInt(IncrementFollowersKey.SelectedKeyName) + 1;
	Blackboard.SetValueAsInt(IncrementFollowersKey.SelectedKeyName, NewFollowersAmount);

	//Set weapon type focus
	Blackboard.SetValueAsBool(WeaponFocusKey.SelectedKeyName, NewWeaponFocusValue);

	//set target field
	if (!Player1->GetAnimationData().IsDustpanPlayer == NewWeaponFocusValue)
	{
		Blackboard.SetValueAsObject(TargetKey.SelectedKeyName, Player1);
	}
	else
	{
		Blackboard.SetValueAsObject(TargetKey.SelectedKeyName, Player2);
	}
}
