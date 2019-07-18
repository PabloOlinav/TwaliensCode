// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "ReportPlayerPosition.generated.h"

/**
 * 
 */
UCLASS()
class TWALIENS_API UReportPlayerPosition : public UBTService
{
	GENERATED_BODY()

public:
	UReportPlayerPosition();

protected:
	virtual void OnGameplayTaskActivated(UGameplayTask & Task) override;
	virtual void OnGameplayTaskDeactivated(UGameplayTask & Task) override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard key")
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard key")
	FBlackboardKeySelector Player1Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard key")
	FBlackboardKeySelector Player1SightedKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard key")
	FBlackboardKeySelector Player1LastPositionKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard key")
	FBlackboardKeySelector Player1ReportedKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard key")
	FBlackboardKeySelector Player2Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard key")
	FBlackboardKeySelector Player2SightedKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard key")
	FBlackboardKeySelector Player2LastPositionKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard key")
	FBlackboardKeySelector Player2ReportedKey;

private:

	void InformAboutPlayer(UBlackboardComponent * Blackboard, FBlackboardKeySelector PlayerSightedKey, FBlackboardKeySelector PlayerKey, FBlackboardKeySelector PlayerLastPositionKey) const;

};
