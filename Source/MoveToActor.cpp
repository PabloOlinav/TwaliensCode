// All rights reserved.

#include "MoveToActor.h"
#include "Runtime/GameplayTasks/Classes/GameplayTask.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/AIModule/Classes/Navigation/PathFollowingComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/Actor.h"

UMoveToActor::UMoveToActor() : StopOnOverlap(false), CanStrafe(false), UsingARAmount(false), UsingARKey(true), RecalculationDistance(100), ShouldUpdate(false)
{
	ForceInstancing(true);
	bNotifyTick = true;
}

void UMoveToActor::OnGameplayTaskActivated(UGameplayTask & Task)
{
}

void UMoveToActor::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData * Blackboard = GetBlackboardAsset();
	if(Blackboard)
	{
		TargetDestination.ResolveSelectedKey(*Blackboard);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("There isnt blackboard asset asociated with this Behaviour tree."));
	}
}

EBTNodeResult::Type UMoveToActor::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Finish latent task
	FinishLatentAbort(OwnerComp);

	AAIController* MyController = OwnerComp.GetAIOwner();
	if (MyController && MyController->GetPathFollowingComponent())
	{
		MyController->GetPathFollowingComponent()->AbortMove(*this, FPathFollowingResultFlags::OwnerFinished);
	}

	//return parent class return value
	return Super::AbortTask(OwnerComp, NodeMemory);
}

EBlackboardNotificationResult UMoveToActor::OnTargetValueChanged(const UBlackboardComponent & Blackboard, FBlackboard::FKey KeyId)
{
	//Get behavior tree we are running
	UBehaviorTreeComponent * MyBehaviourTree = Cast<UBehaviorTreeComponent>(Blackboard.GetBrainComponent());

	//If our target is an actor
	if (TargetDestination.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		//Only could change TargetDestinationKey
		//Due to we are following an actor we need to recalculate path
		ShouldUpdate = true;
	}
	//if our target is a location
	else if (TargetDestination.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		//Only could change TargetDestinationKey
		const FVector NewLocation = Blackboard.GetValueAsVector(TargetDestination.SelectedKeyName);

		//If new location is faraway than threshold we do again pathfinding
		float Debug = FVector::DistSquaredXY(CurrentDestination, NewLocation);
		float Debug2 = FMath::Square(RecalculationDistance);
		if (Debug > Debug2)
		{
			ShouldUpdate = true;
		}
	}

	//if we no have bt at this point we remove all observer from bb. if not, all keeps equal
	return MyBehaviourTree ? EBlackboardNotificationResult::ContinueObserving : EBlackboardNotificationResult::RemoveObserver;
}

FVector UMoveToActor::GetVectorDestination(UBehaviorTreeComponent& OwnerComp)
{
	UBlackboardComponent * Blackboard = OwnerComp.GetBlackboardComponent();
	const FVector Destination = Blackboard->GetValueAsVector(TargetDestination.SelectedKeyName);

	return Destination;
}

AActor * UMoveToActor::GetActorDestination(UBehaviorTreeComponent& OwnerComp)
{
	UBlackboardComponent * Blackboard = OwnerComp.GetBlackboardComponent();
	AActor * Destination = Cast<AActor>(Blackboard->GetValueAsObject(TargetDestination.SelectedKeyName));

	return Destination;
}

void UMoveToActor::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	//Get controller
	AAIController * Controller = OwnerComp.GetAIOwner();

	//Controller not null
	if(Controller)
	{
		//If we need to recalculate path
		if (ShouldUpdate)
		{
			//Set update flag to deactivate value
			ShouldUpdate = false;

			//execute task again
			EBTNodeResult::Type MoveReturnValue = ExecuteTask(OwnerComp, NodeMemory);
			
			//If return value is success failed or abort
			if (MoveReturnValue != EBTNodeResult::InProgress)
			{
				//unregister observers for this task
				OwnerComp.GetBlackboardComponent()->UnregisterObserversFrom(this);
				//finish this task with this value
				FinishLatentTask(OwnerComp, MoveReturnValue);
			}
		}
		//If we reach our destination
		else if (Controller->GetMoveStatus() == EPathFollowingStatus::Type::Idle)
		{
			//unregister observers for this task
			OwnerComp.GetBlackboardComponent()->UnregisterObserversFrom(this);
			//finish this task with success
			FinishLatentTask(OwnerComp, EBTNodeResult::Type::Succeeded);
		}
	}
}

EBTNodeResult::Type UMoveToActor::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UBlackboardComponent * Blackboard = OwnerComp.GetBlackboardComponent();
	UBehaviorTree * BT = OwnerComp.GetCurrentTree();
	FVector NewDestination;
	AActor * NewActorDestination;
	AAIController * Controller = OwnerComp.GetAIOwner();

	if(Controller)
	{
		float Radius;
		
		if (UsingARKey)
		{
			Radius = Blackboard->GetValueAsFloat(AcceptanceRadiusKey.SelectedKeyName);
		}
		else if (UsingARAmount)
		{
			Radius = AcceptanceRadiusAmount;
		}
		else
		{
			return EBTNodeResult::Failed;
		}

		EPathFollowingRequestResult::Type MoveOrderResult;

		if (TargetDestination.SelectedKeyType == UBlackboardKeyType_Object::StaticClass() || TargetDestination.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
		{
			if (TargetDestination.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
			{
				NewActorDestination = GetActorDestination(OwnerComp);
				MoveOrderResult = Controller->MoveToActor(NewActorDestination, Radius, StopOnOverlap, true, CanStrafe, 0, true);
			}
			else
			{
				CurrentDestination = GetVectorDestination(OwnerComp);
				MoveOrderResult = Controller->MoveToLocation(CurrentDestination, Radius, StopOnOverlap, true, false, CanStrafe, 0, true);
			}
			
			Blackboard->RegisterObserver(TargetDestination.GetSelectedKeyID(), this, FOnBlackboardChangeNotification::CreateUObject(this, &UMoveToActor::OnTargetValueChanged));

			if (MoveOrderResult == EPathFollowingRequestResult::AlreadyAtGoal)
			{
				return EBTNodeResult::Type::Succeeded;
			}
			else if (MoveOrderResult == EPathFollowingRequestResult::Failed)
			{
				return EBTNodeResult::Type::Failed;
			}
			else
			{
				return EBTNodeResult::Type::InProgress;
			}
		}
		else
		{
			return EBTNodeResult::Failed;
		}
	}

	return EBTNodeResult::Failed;
}