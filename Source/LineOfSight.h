// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "LineOfSight.generated.h"

/**
 * 
 */
UCLASS()
class TWALIENS_API ULineOfSight : public UBTDecorator
{
	GENERATED_BODY()

public:
	ULineOfSight();

protected:
	virtual void OnGameplayTaskActivated(UGameplayTask & Task) override;
	virtual void OnGameplayTaskDeactivated(UGameplayTask & Task) override;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decorator Property")
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decorator Property")
	TArray<FName> TagsToIgnore;

private:
	bool IsThereLineOfSight(TArray<FHitResult> Hit, const AActor * Target) const;

	bool ActorTagsAreInIgnoreList(TArray<FName> Tags) const;
};
