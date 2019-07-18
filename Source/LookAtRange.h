// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "LookAtRange.generated.h"

/**
 * 
 */
UCLASS()
class TWALIENS_API ULookAtRange : public UBTService
{
	GENERATED_BODY()

public:

	ULookAtRange();
	virtual void OnGameplayTaskActivated(UGameplayTask & Task) override;
	virtual void OnGameplayTaskDeactivated(UGameplayTask & Task) override;

protected:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard key")
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard key")
	FBlackboardKeySelector Player1Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard key")
	FBlackboardKeySelector Player1ReportedKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard key")
	FBlackboardKeySelector Player1ReportAmountKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard key")
	FBlackboardKeySelector Player1SightedKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard key")
	FBlackboardKeySelector Player2Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard key")
	FBlackboardKeySelector Player2ReportedKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard key")
	FBlackboardKeySelector Player2ReportAmountKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard key")
	FBlackboardKeySelector Player2SightedKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Property")
	float SightDistance;

	class AAIController * AIControllerReference;

private:

	bool DoTraceAction(FVector Position, FQuat Rotation, TArray<FOverlapResult> &OverlapResults);

	void SearchForPlayers(UBlackboardComponent * Blackboard, const TArray<FOverlapResult> &OverlapResults);

	void DoTargettingAndRegisterFocus(UBlackboardComponent * Blackboard, class ATwalien * Player);

	void RegisterFocusInAlertSystem(UBlackboardComponent * Blackboard, FBlackboardKeySelector PlayerReportedKey, FBlackboardKeySelector PlayerReportAmountKey, FBlackboardKeySelector OtherPlayerReportedKey, FBlackboardKeySelector OtherPlayerReportAmountKey, FBlackboardKeySelector OtherPlayerSightedKey);

	void IncrementPlayerReference(UBlackboardComponent * Blackboard, FBlackboardKeySelector PlayerReportedKey, FBlackboardKeySelector PlayerReportAmountKey);

	void DecrementPlayerReference(UBlackboardComponent * Blackboard, FBlackboardKeySelector PlayerReportedKey, FBlackboardKeySelector PlayerReportAmountKey, FBlackboardKeySelector PlayerSightedKey);
};
