// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "NeedToChasePlayer.generated.h"

/**
 * 
 */
UCLASS()
class TWALIENS_API UNeedToChasePlayer : public UBTDecorator
{
	GENERATED_BODY()

public:

	UNeedToChasePlayer();
	virtual void OnGameplayTaskActivated(UGameplayTask & Task) override;
	virtual void OnGameplayTaskDeactivated(UGameplayTask & Task) override;

protected:

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackBoard key")
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decorator Property")
	FBlackboardKeySelector MeleeRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decorator Property")
	bool OverlapDistance;

	mutable class AAIController * AIControllerReference;
};
