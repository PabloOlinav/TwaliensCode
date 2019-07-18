// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "ChangeFocus.generated.h"

/**
 * 
 */
UCLASS()
class TWALIENS_API UChangeFocus : public UBTService
{
	GENERATED_BODY()
	
public:
	UChangeFocus();
	virtual void OnGameplayTaskActivated(UGameplayTask & Task) override;
	virtual void OnGameplayTaskDeactivated(UGameplayTask & Task) override;

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Settings")
	bool MustChangeFocusOnSwitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Settings")
	bool MustChangeFocusWithDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Settings")
	float DistanceDelta;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Settings")
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Settings")
	FBlackboardKeySelector WeaponFocusKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Settings")
	FBlackboardKeySelector Player1ReferenceKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Settings")
	FBlackboardKeySelector Player2ReferenceKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Settings")
	FBlackboardKeySelector DustpanFollowersKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Settings")
	FBlackboardKeySelector WeaponFollowersKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Settings")
	FBlackboardKeySelector TargetNearPositionSetKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Settings")
	FBlackboardKeySelector TargetRadialQuadrantKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Settings")
	FBlackboardKeySelector TargetRadialRingKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Settings")
	FBlackboardKeySelector TargetRadialPositionKey;

private:
	void ChangeFocusOnSwitch(class UBlackboardComponent & Blackboard, class ATwalien & Player1, class ATwalien & Player2, const class ATwalien & Target);
	void ChangeFocusOnDistance(class UBehaviorTreeComponent & OwnerComp, class UBlackboardComponent & Blackboard, class ATwalien & Player1, class ATwalien & Player2, const class ATwalien & Target);
	void ChangeNewFocus(const class ATwalien & Target, class UBlackboardComponent & Blackboard, class ATwalien & NewTarget);
	void RegisterFocus(const FBlackboardKeySelector & IncrementFollowers, const FBlackboardKeySelector & DecrementFollowers, class UBlackboardComponent & Blackboard, bool NewWeaponFocusValue);
	void FreeStrategicPointFromTarget(class UBlackboardComponent & Blackboard, class AActor & Target);
};
