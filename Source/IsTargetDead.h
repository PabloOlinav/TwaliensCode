// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "IsTargetDead.generated.h"

/**
 * 
 */
UCLASS()
class TWALIENS_API UIsTargetDead : public UBTDecorator
{
	GENERATED_BODY()

public:
	UIsTargetDead();

	EBlackboardNotificationResult OnBlackboardKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID);

protected:
	virtual void OnGameplayTaskActivated(UGameplayTask & Task) override;
	virtual void OnGameplayTaskDeactivated(UGameplayTask & Task) override;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decorator Property")
	FBlackboardKeySelector TargetKey;

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	bool ShouldAbort(class UBehaviorTreeComponent & OwnerComp);

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;


	FScriptDelegate LifeDelegate;
	class ATwalien * ReferenceToPlayerHPObserve;
	UFUNCTION()
	void OnTargetIsDead();

	class UBehaviorTreeComponent * BehaviourTreeComponent;
};
