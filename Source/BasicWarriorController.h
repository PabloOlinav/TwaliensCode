// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "CommonAIControllerActions.h"
#include "BasicWarriorController.generated.h"

/**
 *
 */
UCLASS()
class TWALIENS_API ABasicWarriorController : public AAIController, public ICommonAIControllerActions
{
	GENERATED_BODY()

public:
	ABasicWarriorController(const class FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Controller Functions")
	bool GetAgentIsAttacking() override;

	UFUNCTION(BlueprintCallable, Category = "Controller Functions")
	virtual void SetAgentIsAttacking(bool NewAttackStatus) override;

	UFUNCTION(BlueprintCallable, Category = "Controller Functions")
	virtual void SetAgentTarget(class AActor * Target) override;

	UFUNCTION(BlueprintCallable, Category = "Controller Functions")
	virtual void RemoveAgentTarget(class AActor * Target) override;

	UFUNCTION(BlueprintCallable, Category = "Controller Functions")
	virtual void ActivateBehaviour();

	UFUNCTION(BLueprintCallable, Category = "Controller Functions")
	virtual void AlertPlayersInSight(bool InSight) override;

	UFUNCTION(BLueprintCallable, Category = "Controller Functions")
	void CancelTarget();

	UFUNCTION(BlueprintCallable)
	void SetAgentInitialPosition(const FVector & Position);

	void OnBasicWarriorBeingDestroyed();

	void StopExecutingAI();

	void RestartExecutingAI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Possess(APawn * InPawn) override;

	// Called once when actor its destroyed
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	class UBehaviorTree * BehaviourTree;
	class UBlackboardData * BlackboardData;
	class ABasicWarrior * BasicWarrior;
	//TODO - Clase cache - a$tuR
	class ACharacterManager * CharacterManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targetting")
	FName TargetKey; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targetting")
	FName TauntSkillForcedTargetKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targetting")
	FName PlayerSightedKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targetting")
	FName Player1Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targetting")
	FName Player2Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targetting")
	FName AttackDistanceKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targetting")
	FName SpawnLocationKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targetting")
	FName WeaponFollowers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targetting")
	FName DustpanFollowers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targetting")
	FName WeaponFocus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targetting")
	FName TargetNearPositionSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targetting")
	FName TargetRadialQuadrant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targetting")
	FName TargetRadialRing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targetting")
	FName TargetRadialPosition;
};