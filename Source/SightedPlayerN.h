// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "SightedPlayerN.generated.h"

/**
 * 
 */
UCLASS()
class TWALIENS_API USightedPlayerN : public UBTDecorator
{
	GENERATED_BODY()

public:
	USightedPlayerN();

protected:
	virtual void OnGameplayTaskActivated(UGameplayTask & Task) override;
	virtual void OnGameplayTaskDeactivated(UGameplayTask & Task) override;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decorator Property")
	FBlackboardKeySelector ChoosenSightKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decorator Property")
	int PlayerIndex;
};
