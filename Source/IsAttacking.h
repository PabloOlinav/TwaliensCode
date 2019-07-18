// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "IsAttacking.generated.h"

/**
 * 
 */
UCLASS()
class TWALIENS_API UIsAttacking : public UBTDecorator
{
	GENERATED_BODY()

public:
	UIsAttacking();

protected:
	virtual void OnGameplayTaskActivated(UGameplayTask & Task) override;
	virtual void OnGameplayTaskDeactivated(UGameplayTask & Task) override;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const override;

private:
	mutable class ICommonAIControllerActions * Controller;
};
