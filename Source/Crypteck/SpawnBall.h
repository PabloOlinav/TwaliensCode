// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActiveCrypteckBall.h"
#include "EnemySpawner.h"
#include "SpawnBall.generated.h"

UCLASS()
class TWALIENS_API ASpawnBall : public AActiveCrypteckBall
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnBall();

private:
	class ACharacterManager * PlayerManager;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnBallHit(const FHitResult & Hit) override;

	virtual void CreateOrbitNewPosition(FVector & Position) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnBall")
	TArray<AEnemySpawner *> SpawnersLinkedToSpawnBall;

	//UNUSED
	virtual void SetPositionToAttackTeleport() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Action(float DeltaSeconds) override;

};
