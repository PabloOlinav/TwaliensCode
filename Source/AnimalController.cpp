// All rights reserved.

#include "AnimalController.h"
#include "ConstructorHelpers.h"
#include "BehaviorTree/BehaviorTree.h"
#include "EngineUtils.h"
#include "LocationManager.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interfaces/EnvironmentalAnimal.h"

AAnimalController::AAnimalController() : Super()
{
	//Search for behaviour tree and blackboard assets
	ConstructorHelpers::FObjectFinder<UBehaviorTree> BTFinder(TEXT("/Game/AI/Animal/BT_AnimalBehaviourTree_0"));
	ConstructorHelpers::FObjectFinder<UBlackboardData> BBFinder(TEXT("/Game/AI/Animal/BB_AnimalBlackBoard_0"));

	//Creating blackboard
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("AnimalControllerBlackboard"));

	//if we found it, we assing them to our variables
	if (BTFinder.Succeeded() && BBFinder.Succeeded())
	{
		BehaviourTree = BTFinder.Object;
		BlackboardData = BBFinder.Object;
	}
}

void AAnimalController::BeginPlay()
{
	Super::BeginPlay();

	//Animals were created to go between severals points with no break, 
	//we need to search in the world for location manager that manages 
	//all points divided by regions
	TActorIterator<ALocationManager> LocationsManager(GetWorld());

	NavPointsManager = *LocationsManager;

	//Initialize blackboard
	UseBlackboard(BlackboardData, Blackboard);

	//If controlled pawn is not null
	if (Animal)
	{
		//Start running behaviour tree
		RunBehaviorTree(BehaviourTree);
	}
}

void AAnimalController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);
	
	Animal = Cast<IEnvironmentalAnimal>(InPawn);

	//If blackboard is initialized, start running behaviour tree
	if (Blackboard)
	{
		RunBehaviorTree(BehaviourTree);
	}
}

void AAnimalController::GetNextNavPoint(FVector & NextLocation) const
{
	if (NavPointsManager && Animal)
	{
		NavPointsManager->GetRandomLocation(NextLocation, Animal->GetRegionIndex());
	}
	else
	{
		NextLocation.Set(INT_MIN, INT_MIN, INT_MIN);
	}
}
