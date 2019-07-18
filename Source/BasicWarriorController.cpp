// All rights reserved.

#include "BasicWarriorController.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTree.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardData.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "CharacterManager.h"
#include "BasicWarrior.h"
#include "Twalien.h"
#include "TargetTauntActor.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Structure/TwalienAnimationData.h"
#include "Components/RadialAttackableLocations.h"

ABasicWarriorController::ABasicWarriorController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent"))), TargetKey("Target"), TauntSkillForcedTargetKey("TauntSkillForcedTarget"), PlayerSightedKey("PlayersSighted"), Player1Key("Player1Reference"), Player2Key("Player2Reference"), AttackDistanceKey("AttackDistance"), SpawnLocationKey("SpawnLocation"), WeaponFollowers("WeaponFollowers"), DustpanFollowers("DustpanFollowers"), WeaponFocus("WeaponFocus"), TargetNearPositionSet("TargetNearPositionSet"), TargetRadialQuadrant("TargetRadialQuadrant"), TargetRadialRing("TargetRadialRing"), TargetRadialPosition("TargetRadialPosition")
{
	BlackboardData = nullptr;
	BehaviourTree = nullptr;

	ConstructorHelpers::FObjectFinder<UBehaviorTree> BTFinder(TEXT("/Game/AI/BasicWarrior/BT_BasicWarriorBehaviourTree_0.BT_BasicWarriorBehaviourTree_0"));
	ConstructorHelpers::FObjectFinder<UBlackboardData> BBFinder(TEXT("/Game/AI/BasicWarrior/BB_BasicWarriorBlackboard_0.BB_BasicWarriorBlackboard_0"));
	
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("Blackboard");
	
	if (BTFinder.Succeeded() && BBFinder.Succeeded())
	{
		BlackboardData = BBFinder.Object;
		BehaviourTree = BTFinder.Object;
	}

	BasicWarrior = nullptr;
}

// Called when the game starts or when spawned
void ABasicWarriorController::BeginPlay()
{ 
	Super::BeginPlay();

	UseBlackboard(BlackboardData, Blackboard);

	TActorIterator<ACharacterManager> Manager(GetWorld());
	CharacterManager = *Manager;
	
	Blackboard->SetValueAsObject(TargetKey, nullptr);
	Blackboard->SetValueAsObject(TauntSkillForcedTargetKey, nullptr);
	if(!Blackboard->GetValueAsObject(Player1Key))
	{
		Blackboard->SetValueAsObject(Player1Key, CharacterManager->GetPlayer(0));
		Blackboard->SetValueAsObject(Player2Key, CharacterManager->GetPlayer(1));
	}

	if (BasicWarrior)
	{
		Blackboard->SetValueAsFloat(AttackDistanceKey, BasicWarrior->GetAttackDistance());
		Blackboard->SetValueAsBool(PlayerSightedKey, false);
		RunBehaviorTree(BehaviourTree);
	}
}

void ABasicWarriorController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

}

void ABasicWarriorController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);

	BasicWarrior = Cast<ABasicWarrior>(InPawn);

	//If Blackboard is not nullptr, BasicWarrior was spawned manually
	if(Blackboard)
	{
		Blackboard->SetValueAsBool(PlayerSightedKey, false);
		Blackboard->SetValueAsFloat(AttackDistanceKey, BasicWarrior->GetAttackDistance());
		RunBehaviorTree(BehaviourTree);
	}
}

bool ABasicWarriorController::GetAgentIsAttacking()
{
	return BasicWarrior->GetIsAttacking();
}

void ABasicWarriorController::SetAgentIsAttacking(bool NewAttackStatus)
{
	BasicWarrior->SetIsAttacking(NewAttackStatus);
}

void ABasicWarriorController::SetAgentTarget(AActor * Target)
{
	//Check if target is a taunt object or a player
	ATargetTauntActor * SkillTaunt = Cast<ATargetTauntActor>(Target);
	
	//If not null target is a taunt object
	if (SkillTaunt)
	{
		//if this agent is not taunt by any other taunt
		if (!Blackboard->GetValueAsObject(TauntSkillForcedTargetKey))
		{
			//Set target
			Blackboard->SetValueAsObject(TauntSkillForcedTargetKey, SkillTaunt);
		}
	}
	//if skilltaunt is null, the target is a player
	else 
	{
		//check is a player
		ATwalien * Player = Cast<ATwalien>(Target);
		if (Player)
		{
			//set target field
			Blackboard->SetValueAsObject(TargetKey, Player);
			//set action flag to true if it wasnt
			if (!Blackboard->GetValueAsBool(PlayerSightedKey))
			{
				Blackboard->SetValueAsBool(PlayerSightedKey, true);
			}
			FTwalienAnimationData AnimData = Player->GetAnimationData();
			//set target weapon
			Blackboard->SetValueAsBool(WeaponFocus, !AnimData.IsDustpanPlayer);
			//increment followers
			if (AnimData.IsDustpanPlayer)
			{
				Blackboard->SetValueAsInt(DustpanFollowers, Blackboard->GetValueAsInt(DustpanFollowers) + 1);
			}
			else
			{
				Blackboard->SetValueAsInt(WeaponFollowers, Blackboard->GetValueAsInt(WeaponFollowers) + 1);
			}
		}
	}
}

void ABasicWarriorController::RemoveAgentTarget(AActor * Target)
{
	ATargetTauntActor * SkillTaunt = Cast<ATargetTauntActor>(Target);

	if (SkillTaunt)
	{
		if (Blackboard->GetValueAsObject(TauntSkillForcedTargetKey))
		{
			Blackboard->SetValueAsObject(TauntSkillForcedTargetKey, nullptr);
		}
	}
	else
	{
		ATwalien * Player = Cast<ATwalien>(Target);
		if (Player)
		{
			CancelTarget();
		}
	}
}

void ABasicWarriorController::ActivateBehaviour()
{
	//If Blackboard is not nullptr, BasicWarrior was spawned manually
	if (Blackboard)
	{
		Blackboard->SetValueAsVector("SpawnLocation", GetPawn()->GetActorLocation());

		RunBehaviorTree(BehaviourTree);
	}
}

void ABasicWarriorController::OnBasicWarriorBeingDestroyed()
{
	//Cancel target
	CancelTarget();

	//Stop behaviour tree
	StopExecutingAI();
}

void ABasicWarriorController::StopExecutingAI()
{
	UBehaviorTreeComponent * MyBT = Cast<UBehaviorTreeComponent>(BrainComponent);

	if (MyBT)
	{
		MyBT->StopTree(EBTStopMode::Forced);
	}
}

void ABasicWarriorController::RestartExecutingAI()
{
	UBehaviorTreeComponent * MyBT = Cast<UBehaviorTreeComponent>(BrainComponent);

	if (MyBT)
	{
		MyBT->StartTree(*BehaviourTree, EBTExecutionMode::Looped);
	}
}

void ABasicWarriorController::AlertPlayersInSight(bool InSight)
{
	Blackboard->SetValueAsBool(PlayerSightedKey, InSight);
}

void ABasicWarriorController::CancelTarget()
{
	ATwalien * Player = Cast<ATwalien>(Blackboard->GetValueAsObject(TargetKey));

	//if we have target
	if (Player)
	{
		//If our target have weapon
		if (Blackboard->GetValueAsBool(WeaponFocus))
		{
			//Get weapon followers and decrement it by 1
			Blackboard->SetValueAsInt(WeaponFollowers, Blackboard->GetValueAsInt(WeaponFollowers) - 1);
		}
		else
		{
			//Get dustpan followers and decrement it by 1
			Blackboard->SetValueAsInt(DustpanFollowers, Blackboard->GetValueAsInt(DustpanFollowers) - 1);
		}
		//if we chased player
		if(Blackboard->GetValueAsBool(TargetNearPositionSet))
		{
			//free relative position and flag
			UActorComponent * Component = Player->GetComponentByClass(URadialAttackableLocations::StaticClass());
			if (Component)
			{
				URadialAttackableLocations * StrategicLocationComponent = Cast<URadialAttackableLocations>(Component);
				if (StrategicLocationComponent)
				{
					//free position
					StrategicLocationComponent->FreeRelativePosition(
						FRelativePositionId(Blackboard->GetValueAsInt(TargetRadialQuadrant), Blackboard->GetValueAsInt(TargetRadialRing), Blackboard->GetValueAsInt(TargetRadialPosition))
					);

					//set get strategic position flag to false
					Blackboard->SetValueAsBool(TargetNearPositionSet, false);
				}
			}
		}
		//set target to nullptr
		Blackboard->SetValueAsObject(TargetKey, nullptr);
	}
}

void ABasicWarriorController::SetAgentInitialPosition(const FVector & Position)
{
	Blackboard->SetValueAsVector(SpawnLocationKey, Position);
}
