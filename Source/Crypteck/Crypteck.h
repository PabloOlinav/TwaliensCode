// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Crypteck.generated.h"

UCLASS()
class TWALIENS_API ACrypteck : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACrypteck();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Crypteck")
	class UCapsuleComponent * Capsule;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Crypteck")
	class USkeletalMeshComponent * Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Crypteck")
	class ULifeComponent * Life;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crypteck")
	//Speed of falling movements
	float FallingToCenterSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crypteck")
	//CD Time for Sting Throwing.
	float TimeForStingThrowing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crypteck")
	//CD Time for Smash ball effect.
	float SmashBallCD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crypteck")
	//CD Time for Toxic ball effect.
	float ToxicBallCD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crypteck")
	//CD Time for Fire ball effect.
	float FireBallCD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crypteck")
	//CD Time for Spawn ball effect.
	float SpawnBallCD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crypteck")
	bool DemoVersion;

private:

	/*                       */
	/*    Cryptecks Sting    */
	/*                       */
	//Offset Vector for spawn Stings
	FVector StingThrowOffset;
	//Function that activates sting throwing to player
	void ThrowSting();
	//For getting weapon and dustpan players.
	class ACharacterManager * PlayerManager;
	//Timer For sting CD
	struct FTimerHandle StingCDTime;

	/*                    */
	/*   Crypteck ball    */
	/*                    */

	class ASmashBall * Smash;
	class AToxicBall * Toxic;
	class ASpawnBall * Spawn;
	class AFireBall * Fire;
	FTimerHandle SmashBallTimer, ToxicBallTimer, FireBallTimer, SpawnBallTimer;

	void SetTargetToSmashBall();
	void SetTargetToToxicBall();
	void SetTargetToFireBall();
	void DoSpawnBallEffect();

	/*                       */
	/* Crypteck phases stuff */
	/*                       */

	//Enum that represent Crypteck phases
	enum CrypteckPhases
	{
		DEACTIVATED, PHASE_BALLSMASHTOXIC, PHASE_BALLSPAWN, PHASE_BALLFIRE, PHASE_REVENGE
	};
	//Variable for controlling Crypteck phases
	CrypteckPhases Phase;
	

	UFUNCTION()
	//Delegate for smash sphere life change.
	void OnSmashSphereGetDamage();
	UFUNCTION()
	//Delegate for Fire sphere life change.
	void OnFireSphereGetDamage();
	UFUNCTION()
	//Delegate for toxic sphere life change.
	void OnToxicSphereGetDamage();
	UFUNCTION()
	//Delegate for spawn sphere life change.
	void OnSpawnSphereGetDamage();

	UFUNCTION()
	//Delegate for smash sphere death.
	void OnSmashSphereDeath();
	UFUNCTION()
	//Delegate for fire sphere death.
	void OnFireSphereDeath();
	UFUNCTION()
	//Delegate for toxic sphere death.
	void OnToxicSphereDeath();
	UFUNCTION()
	//Delegate for spawn sphere death.
	void OnSpawnSphereDeath();

	UFUNCTION()
	void OnCrypteckDeath();

	//Variables for controlling crypteck ball dead status
	bool SmashDead, FireDead, ToxicDead, SpawnDead;

	//Auxiliar function that calculate life sphere percentage
	float GetSphereLifeMinimumPercentage(const class ULifeComponent * SphereLife);
	
	//do phase changes calculations
	void TriggerPhaseChange(float NewLocalMinimumPercentageSphereHP);

	/*               */
	/* FALLING STUFF */
	/*               */

	//Last flying position
	FVector OriginPosition;
	//Destination position of falling action
	FVector FallingDestination;
	//Current amount of falling time
	float FallingTime;
	//For movement in a certain amount of time
	float FallingTimeMultiplier;

	/*                */
	/* AUXILIAR STUFF */
	/*                */
	float GetFreeAngle(class AActiveCrypteckBall * NextBall, class AActiveCrypteckBall * PreviousBall, class AActiveCrypteckBall * SelfBall);

	/*                    */
	/*   CRYPTECK LEVEL   */
	/*                    */
	class ACryptecksLevel * Level;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	//function for cryptecks wake up 
	void Activate();

	//Function for cryptecks balls exclusively, get a non blocking orbit angle
	float AssignFreeAngle(class AActiveCrypteckBall * MyBall);

	//Set link between crypteck and its balls
	void SetupBallLinks(class AToxicBall * ToxicBall, class ASmashBall * SmashBall, class AFireBall * FireBall, class ASpawnBall * SpawnBall);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/*                    */
	/*   Crypteck ball    */
	/*                    */
	//Event that must be called by crypteck ball when they go to orbit state
	void OnBallBeginOrbit(class AActiveCrypteckBall * Ball);

	FORCEINLINE class ULifeComponent * GetCryptecksLife() { return Life; }
};
