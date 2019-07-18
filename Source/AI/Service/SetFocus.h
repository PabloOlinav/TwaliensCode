// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SetFocus.generated.h"

/**
 * 
 */
UCLASS()
class TWALIENS_API USetFocus : public UBTService
{
	GENERATED_BODY()

public:

	USetFocus();
	virtual void OnGameplayTaskActivated(UGameplayTask & Task) override;
	virtual void OnGameplayTaskDeactivated(UGameplayTask & Task) override;

protected:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard key")
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard key")
	FBlackboardKeySelector PlayerSightedKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard key")
	FBlackboardKeySelector Player1ReferenceKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard key")
	FBlackboardKeySelector Player2ReferenceKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard key")
	FBlackboardKeySelector WeaponFollowersKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard key")
	FBlackboardKeySelector DustpanFollowersKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard key")
	FBlackboardKeySelector WeaponFocusKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard key")
	FBlackboardKeySelector TargetNearPositionSetKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard key")
	FBlackboardKeySelector TargetRadialQuadrantKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard key")
	FBlackboardKeySelector TargetRadialRingKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard key")
	FBlackboardKeySelector TargetRadialPositionKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Property")
	float FocusRatio;

private:

	class ATwalien * Player1, * Player2;

	void CancelTarget(class UBlackboardComponent & Blackboard);

	void ChooseTarget(class UBlackboardComponent & Blackboard);

	void RegisterFocus(class UBlackboardComponent & Blackboard, FBlackboardKeySelector IncrementFollowersKey, bool NewWeaponFocusValue);

};
