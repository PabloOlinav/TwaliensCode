// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AnimalController.generated.h"

/**
 * 
 */
UCLASS()
class TWALIENS_API AAnimalController : public AAIController
{
	GENERATED_BODY()
	
public:
	AAnimalController();

	virtual void Possess(APawn* InPawn) override;

	void GetNextNavPoint(FVector & NextLocation) const;

protected:
	virtual void BeginPlay() override;

	class UBlackboardData * BlackboardData;
	
	class UBehaviorTree * BehaviourTree;

	class ALocationManager * NavPointsManager;

	class IEnvironmentalAnimal * Animal;
};
