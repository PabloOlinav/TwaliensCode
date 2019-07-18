// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SelectSight.generated.h"

/**
 * 
 */
UCLASS()
class TWALIENS_API USelectSight : public UBTService
{
	GENERATED_BODY()

public:
	USelectSight();

protected:
	virtual void OnGameplayTaskActivated(UGameplayTask & Task) override;
	virtual void OnGameplayTaskDeactivated(UGameplayTask & Task) override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard key")
	FBlackboardKeySelector Player1SightedKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard key")
	FBlackboardKeySelector Player2SightedKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard key")
	FBlackboardKeySelector Player1LastPositionKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard key")
	FBlackboardKeySelector Player2LastPositionKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard key")
	FBlackboardKeySelector ChoosenSightKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard key", meta = (ClampMin = "0", ClampMax = "1"))
	float DeltaSpace = 0.16f;

private:

	void FocusChange(UBlackboardComponent * Blackboard, FVector NPCPosition, FVector CurrentFocusPosition, FVector OtherPosition, int NewValue) const;
};
