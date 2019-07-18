// All rights reserved.

#include "GetRandomLocation.h"
#include "AnimalController.h"
#include "BehaviorTree/BlackboardComponent.h"

UGetRandomLocation::UGetRandomLocation()
{
	ForceInstancing(true);
}

void UGetRandomLocation::OnGameplayTaskActivated(UGameplayTask& Task)
{	
}

EBTNodeResult::Type UGetRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	//If we dont set a valid reference to animal controller
	if (!AnimalController)
	{
		//set a valid one
		AnimalController = Cast<AAnimalController>(OwnerComp.GetAIOwner());
	}

	//If we have a valid reference
	if (AnimalController)
	{
		//Get blackboard
		UBlackboardComponent * Blackboard = OwnerComp.GetBlackboardComponent();
		FVector Destination;

		//Get a random next position from controller callback
		AnimalController->GetNextNavPoint(Destination);

		//When GetNextNavPoint fails return a FVector(INT_MIN, INT_MIN, INT_MIN)
		//If any component of vector is not equal to INT_MIN GetNextNavPoint return a valid value
		if (Destination.X != INT_MIN || Destination.Y != INT_MIN || Destination.Z != INT_MIN)
		{
			//set blackboard value with function return value
			Blackboard->SetValueAsVector(DestinationKey.SelectedKeyName, Destination);
			//Return success
			return EBTNodeResult::Succeeded;
		}
		else
		{
			//If return value is not valid return fail
			return EBTNodeResult::Failed;
		}
	}
	else
	{
		//If controller reference is not valid, return fail
		return EBTNodeResult::Failed;
	}
}
