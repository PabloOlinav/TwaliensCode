// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "HasStrategicPoint.generated.h"

/**
 * 
 */
UCLASS()
class TWALIENS_API UHasStrategicPoint : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UHasStrategicPoint();

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;
	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override;

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Settings")
	FBlackboardKeySelector TargetNearPositionSetKey;
};
