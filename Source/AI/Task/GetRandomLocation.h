// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GetRandomLocation.generated.h"

/**
 * 
 */
UCLASS()
class TWALIENS_API UGetRandomLocation : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UGetRandomLocation();

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Settings")
	FBlackboardKeySelector DestinationKey;

	class AAnimalController * AnimalController;

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
