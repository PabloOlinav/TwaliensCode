// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "InformTargetDisappear.generated.h"

/**
 * 
 */
UCLASS()
class TWALIENS_API UInformTargetDisappear : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UInformTargetDisappear();

protected:
	virtual void OnGameplayTaskActivated(class UGameplayTask & Task) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Task Property")
	FBlackboardKeySelector Player1SightedKey;
	UPROPERTY(EditAnywhere, Category = "Task Property")
	FBlackboardKeySelector Player1ReportedKey;
	UPROPERTY(EditAnywhere, Category = "Task Property")
	FBlackboardKeySelector Player1ReportAmountKey;
	UPROPERTY(EditAnywhere, Category = "Task Property")
	FBlackboardKeySelector Player2SightedKey;
	UPROPERTY(EditAnywhere, Category = "Task Property")
	FBlackboardKeySelector Player2ReportedKey;
	UPROPERTY(EditAnywhere, Category = "Task Property")
	FBlackboardKeySelector Player2ReportAmountKey;

private:
	void DecrementPlayerReference(class UBlackboardComponent * Blackboard, const FBlackboardKeySelector & PlayerReportedKey, const FBlackboardKeySelector & PlayerReportAmountKey, const FBlackboardKeySelector & PlayerSightedKey);

};
