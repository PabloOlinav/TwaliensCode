// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CrypteckBall.h"
#include "ActiveCrypteckBall.h"
#include "SmashBall.generated.h"

UCLASS()
class TWALIENS_API ASmashBall : public AActiveCrypteckBall
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASmashBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnBallHit(const FHitResult & Hit) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SmashBall")
	float RestTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SmashBall")
	float Damage;

	virtual void CreateOrbitNewPosition(FVector & Position) override;

	virtual void Move(float DeltaSeconds) override;

	virtual void SetPositionToAttackTeleport() override;

private:
	UFUNCTION()
	void ActionEnded();

	FTimerHandle ActionTimer;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Action(float DeltaSeconds) override;
};
