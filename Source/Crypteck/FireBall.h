// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActiveCrypteckBall.h"
#include "FireBall.generated.h"

UCLASS()
class TWALIENS_API AFireBall : public AActiveCrypteckBall
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFireBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnBallHit(const FHitResult & Hit) override;

	virtual void CreateOrbitNewPosition(FVector & Position) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireBall")
	float FireReleaseTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireBall")
	FVector FallDeviation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SmashBall")
	float Damage;

	virtual void Move(float DeltaSeconds) override;

	virtual void SetPositionToAttackTeleport() override;

private:
	class ABurningArea * ConstructingArea;
	float Time;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Action(float DeltaSeconds) override;

};
