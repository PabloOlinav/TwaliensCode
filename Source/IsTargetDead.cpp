// All rights reserved.

#include "IsTargetDead.h"
#include "Twalien.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "LifeFunctionality.h"
#include "SharedLifeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"

UIsTargetDead::UIsTargetDead()
{
	ForceInstancing(true);
	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;
	LifeDelegate.BindUFunction(this, FName("OnTargetIsDead"));
}

void UIsTargetDead::OnGameplayTaskActivated(UGameplayTask & Task)
{	
}

void UIsTargetDead::OnGameplayTaskDeactivated(UGameplayTask & Task)
{
}

bool UIsTargetDead::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	const UBlackboardComponent * Blackboard = OwnerComp.GetBlackboardComponent();
	ATwalien * Player = Cast<ATwalien>(Blackboard->GetValueAsObject(TargetKey.SelectedKeyName));
	bool IsDead = false;
	if (Player)
	{
		const ILifeFunctionality * Life = Player->GetLifeComponent();
		if (Life)
		{
			IsDead = Life->GetHitPoints() <= 0;
		}
	}

	return IsDead;
}

void UIsTargetDead::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	
	UBlackboardComponent * MyBlackboard = OwnerComp.GetBlackboardComponent();
	UObject * TargetBBData = MyBlackboard->GetValueAsObject(TargetKey.SelectedKeyName);

	if (TargetBBData)
	{
		ATwalien * TargetPlayer = Cast<ATwalien>(TargetBBData);
		TargetPlayer->GetLifeComponent()->AddDelegateToHPUpdate(LifeDelegate);
		ReferenceToPlayerHPObserve = TargetPlayer;
		BehaviourTreeComponent = &OwnerComp;
	}

	if (FBlackboard::InvalidKey != TargetKey.GetSelectedKeyID())
	{
		MyBlackboard->RegisterObserver(TargetKey.GetSelectedKeyID(), this, FOnBlackboardChangeNotification::CreateUObject(this, &UIsTargetDead::OnBlackboardKeyValueChange));
	}
}

void UIsTargetDead::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);
	
	UBlackboardComponent * MyBlackboard = OwnerComp.GetBlackboardComponent();
	UObject * TargetBBData = MyBlackboard->GetValueAsObject(TargetKey.SelectedKeyName);

	if (TargetBBData)
	{
		ATwalien * TargetPlayer = Cast<ATwalien>(TargetBBData);
		check(!ReferenceToPlayerHPObserve || ReferenceToPlayerHPObserve == TargetPlayer);
		TargetPlayer->GetLifeComponent()->RemoveDelegateFromHPUpdate(LifeDelegate);
	}

	MyBlackboard->UnregisterObserversFrom(this);
}


void UIsTargetDead::OnTargetIsDead()
{
	if (BehaviourTreeComponent && ShouldAbort(*BehaviourTreeComponent))
	{
		BehaviourTreeComponent->RequestExecution(this);
	}
}

bool UIsTargetDead::ShouldAbort(class UBehaviorTreeComponent & OwnerComp)
{
	bool Abort = false;
	const bool IsOnActiveBranch = OwnerComp.IsExecutingBranch(this, GetChildIndex());
	
	if (IsOnActiveBranch)
	{
		Abort = (FlowAbortMode == EBTFlowAbortMode::Self || FlowAbortMode == EBTFlowAbortMode::Both) && CalculateRawConditionValue(OwnerComp, nullptr) == IsInversed();
	}
	else
	{
		Abort = (FlowAbortMode == EBTFlowAbortMode::LowerPriority || FlowAbortMode == EBTFlowAbortMode::Both) && CalculateRawConditionValue(OwnerComp, nullptr) != IsInversed();
	}

	return Abort;
}

void UIsTargetDead::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	
	UBlackboardData * MyBlackboardData = Asset.BlackboardAsset;

	if (MyBlackboardData)
	{
		TargetKey.ResolveSelectedKey(*MyBlackboardData);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("There isnt blackboard asset asociated with this Behaviour tree."));
	}
}

EBlackboardNotificationResult UIsTargetDead::OnBlackboardKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
	//Get behavior tree we are running
	UBehaviorTreeComponent * MyBehaviourTree = Cast<UBehaviorTreeComponent>(Blackboard.GetBrainComponent());

	//check bt not null and we should abort
	if (MyBehaviourTree && ShouldAbort(*MyBehaviourTree))
	{
		MyBehaviourTree->RequestExecution(this);
	}

	//if target changed to another player, we remove our previous delegate and observe a new lifecomponent
	UBlackboardComponent * MyBlackboard = MyBehaviourTree->GetBlackboardComponent();

	if (MyBlackboard && ReferenceToPlayerHPObserve)
	{
		//We only have one bb key, so we assume that only can change targetkey
		UObject * TargetBBData = MyBlackboard->GetValueAsObject(TargetKey.SelectedKeyName);
		
		//remove delegate from previous observed life component and set to nullptr reference to player observed hp
		ReferenceToPlayerHPObserve->GetLifeComponent()->RemoveDelegateFromHPUpdate(LifeDelegate);
		ReferenceToPlayerHPObserve = nullptr;
		
		//if we have new target
		if (TargetBBData)
		{
			ATwalien * TargetPlayer = Cast<ATwalien>(TargetBBData);

			if (TargetPlayer)
			{
				ReferenceToPlayerHPObserve = TargetPlayer;
				TargetPlayer->GetLifeComponent()->AddDelegateToHPUpdate(LifeDelegate);
			}
		}
	}

	//if we no have bt at this point we remove all observer from bb. if not, all keeps equal
	return MyBehaviourTree ? EBlackboardNotificationResult::ContinueObserving : EBlackboardNotificationResult::RemoveObserver;
}
