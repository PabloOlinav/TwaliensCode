// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MoveToActor.generated.h"

/**
 * 
 */
UCLASS()
class TWALIENS_API UMoveToActor : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UMoveToActor();

protected:
	virtual void OnGameplayTaskActivated(class UGameplayTask & Task) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	EBlackboardNotificationResult OnTargetValueChanged(const UBlackboardComponent & Blackboard, FBlackboard::FKey KeyId);

	UPROPERTY(EditAnywhere, Category = "Blackboard key")
	FBlackboardKeySelector TargetDestination;

	FVector CurrentDestination;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Property")
	FBlackboardKeySelector AcceptanceRadiusKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Property")
	bool UsingARKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Property")
	float AcceptanceRadiusAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Property")
	bool UsingARAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Property")
	float RecalculationDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Property")
	bool StopOnOverlap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Property")
	bool CanStrafe;

private:
	FVector GetVectorDestination(UBehaviorTreeComponent& OwnerComp);
	class AActor * GetActorDestination(UBehaviorTreeComponent& OwnerComp);
	bool ShouldUpdate;
};