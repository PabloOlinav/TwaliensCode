// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "IsAgentDead.generated.h"

/**
 * 
 */
UCLASS()
class TWALIENS_API UIsAgentDead : public UBTDecorator
{
	GENERATED_BODY()

public:
	UIsAgentDead();

protected:
	virtual void OnGameplayTaskActivated(UGameplayTask & Task) override;
	virtual void OnGameplayTaskDeactivated(UGameplayTask & Task) override;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const override;
};
