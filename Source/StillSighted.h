// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "StillSighted.generated.h"

/**
 * 
 */
UCLASS()
class TWALIENS_API UStillSighted : public UBTDecorator
{
	GENERATED_BODY()

public:
	UStillSighted();

protected:
	virtual void OnGameplayTaskActivated(UGameplayTask & Task) override;
	virtual void OnGameplayTaskDeactivated(UGameplayTask & Task) override;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decorator Property")
	FBlackboardKeySelector PlayerSightedKey;
};
