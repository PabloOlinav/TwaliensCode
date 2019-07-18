// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "UpdateStrategicPoint.generated.h"

/**
 * 
 */
UCLASS()
class TWALIENS_API UUpdateStrategicPoint : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UUpdateStrategicPoint();

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Settings")
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Settings")
	FBlackboardKeySelector TargetNearPositionKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Settings")
	FBlackboardKeySelector TargetNearPositionSetKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Settings")
	FBlackboardKeySelector TargetRadialQuadrantKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Settings")
	FBlackboardKeySelector TargetRadialRingKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Settings")
	FBlackboardKeySelector TargetRadialPositionKey;

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	class URadialAttackableLocations * GetTargetComponent(const class AActor & Target) const;
};
